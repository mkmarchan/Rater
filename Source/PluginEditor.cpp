/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RaterAudioProcessorEditor::RaterAudioProcessorEditor (RaterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (100, 400);
    
    rate.setSliderStyle (Slider::LinearBarVertical);
    rate.setRange(p.minRate, p.maxRate, 0.001);
    rate.setTextBoxStyle (Slider::TextBoxBelow, false, getWidth() / 2, 40);
    rate.setPopupDisplayEnabled(false, false, this);
    rate.setTextValueSuffix (" Rate");
    rate.setValue(p.rate);
    rate.setSkewFactorFromMidPoint(1.0);
    // this function adds the slider to the editor
    addAndMakeVisible (&rate);
    addAndMakeVisible(&debugText);
    
    rate.addListener (this);
    
    grainDur.setSliderStyle (Slider::LinearBarVertical);
    grainDur.setRange(p.minDur, p.maxDur, 2);
    grainDur.setTextBoxStyle (Slider::TextBoxBelow, false, getWidth() / 2, 40);
    grainDur.setPopupDisplayEnabled(false, false, this);
    grainDur.setTextValueSuffix (" Grain Duration");
    grainDur.setValue(p.grainDur);
    grainDur.setSkewFactorFromMidPoint(p.maxDur / 2);
    // this function adds the slider to the editor
    addAndMakeVisible (&grainDur);
    
    grainDur.addListener (this);
}

RaterAudioProcessorEditor::~RaterAudioProcessorEditor()
{
}

//==============================================================================
void RaterAudioProcessorEditor::paint (Graphics& g)
{
    // fill the whole window white
    g.fillAll (Colours::black);
    // set the current drawing colour to black
    g.setColour (Colours::black);
    // set the font size and draw text to the screen
    g.setFont (15.0f);
    //g.drawFittedText ("Rater", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void RaterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    debugText.setBounds(0, 0, getWidth(), 30);
    rate.setBounds(getWidth() / 4, 30, getWidth() / 4, getHeight() - 60);
    grainDur.setBounds(getWidth() * 2/ 4, 30, getWidth() / 4, getHeight() - 60);
}

void RaterAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    debugText.setText((String) processor.freq, dontSendNotification);
    if (slider == &rate) {
        processor.rate = rate.getValue();
    } else if (slider == &grainDur) {
        float grainFreq = (grainDur.getValue() / 44100.0);
        processor.grainDur = grainDur.getValue();
        //processor.grain[0].setParams(2.0 * grainFreq, grainFreq, 1.0);
        //processor.grain[1].setParams(2.0 * grainFreq, grainFreq, 1.0);
    }
}
