/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayBuffer.h"
#include "SyncGrain.h"
#include "PitchTracker.h"

//==============================================================================
/**
*/
class RaterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    RaterAudioProcessor();
    ~RaterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float rate;
    float minRate;
    float maxRate;
    int minDur;
    int maxDur;
    int grainDur;
    float freq;
    
    juce::dsp::FFT fft;
    
    SyncGrain grain[2];
    


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RaterAudioProcessor)
    int sampleWrap(int sample);
    
    DelayBuffer grainBuf;
    std::unique_ptr<PitchTracker> pTracker;
    int counter;
    int lastTrigger;
    int maxBufLen;
    float fftData[4096];
    juce::dsp::LookupTable<float> hann;
    
    
};
