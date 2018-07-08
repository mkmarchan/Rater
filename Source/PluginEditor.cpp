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
    rate.setValue(1.0);
    rate.setSkewFactorFromMidPoint(1.0);
    // this function adds the slider to the editor
    addAndMakeVisible (&rate);
    
    rate.addListener (this);
}

RaterAudioProcessorEditor::~RaterAudioProcessorEditor()
{
}

//==============================================================================
void RaterAudioProcessorEditor::paint (Graphics& g)
{
    // fill the whole window white
    g.fillAll (Colours::white);
    // set the current drawing colour to black
    g.setColour (Colours::black);
    // set the font size and draw text to the screen
    g.setFont (15.0f);
    g.drawFittedText ("Rater", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void RaterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    rate.setBounds(getWidth() / 4, 30, getWidth() / 2, getHeight() - 60);
}

void RaterAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    processor.rate = slider->getValue();
}
