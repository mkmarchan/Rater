/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RaterAudioProcessor::RaterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    rate = 1.0;
    minRate = 0.25;
    maxRate = 4.0;
    // TODO: Issue with odd length grain durations
    minDur = 220;
    maxDur = 44100;
    grainDur = maxDur;
    counter = 0;
    lastTrigger = 0;
    maxBufLen = ceil(std::max(abs(1 - minRate), abs(1 - maxRate))) * maxDur;
    grainBuf.setSize(2, maxBufLen);
    grainBuf.clear();
    
    int grainLen = maxDur;
    hann.initialise([grainLen](size_t i) {return (1.0 - cos(2 * M_PI * i / (grainLen - 1))) * 0.5;}, maxDur);
}

RaterAudioProcessor::~RaterAudioProcessor()
{
}

//==============================================================================
const String RaterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RaterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RaterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RaterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RaterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RaterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RaterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RaterAudioProcessor::setCurrentProgram (int index)
{
}

const String RaterAudioProcessor::getProgramName (int index)
{
    return {};
}

void RaterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void RaterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    counter = 0;
    lastTrigger = 0;
}

void RaterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    grainBuf.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RaterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RaterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    int initCount = counter;
    int initTrigger = lastTrigger;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* inBuffer = buffer.getReadPointer(channel);
        auto* outBuffer = buffer.getWritePointer(channel);
        auto* grainBuffer = grainBuf.getWritePointer(channel);
        
        counter = initCount;
        lastTrigger = initTrigger;
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            if (counter % (grainDur / 2) == 0) {
                lastTrigger = counter;
            }
            int samplesSinceTrigger = sampleWrap(counter - lastTrigger);
            int rateOffset = rate > 1.0 ? (rate - 1.0) * grainDur : 0;
            int g1Sample, g2Sample;
            
            if (lastTrigger % grainDur == 0) {
                g1Sample = lastTrigger + samplesSinceTrigger * rate - rateOffset;
                g2Sample = lastTrigger + (samplesSinceTrigger + grainDur / 2) * rate - rateOffset - grainDur / 2;
            } else {
                g1Sample = lastTrigger + (samplesSinceTrigger + grainDur / 2) * rate - rateOffset - grainDur / 2;
                g2Sample = lastTrigger + samplesSinceTrigger * rate - rateOffset;
            }
            
            g1Sample = sampleWrap(g1Sample);
            g2Sample = sampleWrap(g2Sample);
            grainBuffer[counter % maxBufLen] = inBuffer[sample];
            outBuffer[sample] = grainBuffer[g1Sample] * hann.getUnchecked((counter % grainDur) / (float)grainDur * maxDur)
                + grainBuffer[g2Sample] * hann.getUnchecked(((counter + grainDur / 2) % grainDur) / (float)grainDur * maxDur);
            
            // TODO: THIS WILL OVERFLOW AFTER 13 HOURS
            counter++;
        }
    }
}

//==============================================================================
bool RaterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RaterAudioProcessor::createEditor()
{
    return new RaterAudioProcessorEditor (*this);
}

//==============================================================================
void RaterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RaterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RaterAudioProcessor();
}

int RaterAudioProcessor::sampleWrap(int sample)
{
    while (sample < maxBufLen) sample += maxBufLen;
    return sample % maxBufLen;
}
