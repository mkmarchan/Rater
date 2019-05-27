/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RaterAudioProcessorEditor  : public AudioProcessorEditor,
                                   private Slider::Listener
{
public:
    RaterAudioProcessorEditor (RaterAudioProcessor&);
    ~RaterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged (Slider* slider) override;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RaterAudioProcessor& processor;
    Label debugText;
    Slider rate;
    Slider grainDur;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RaterAudioProcessorEditor)
};
