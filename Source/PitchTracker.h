/*
  ==============================================================================

    PitchTracker.h
    Created: 26 May 2019 3:24:43pm
    Author:  Mick Marchan

  ==============================================================================
*/
#pragma once

#include "DelayBuffer.h"

class PitchTracker {
public:
    PitchTracker(int sampleRate);
    ~PitchTracker();
    
    void put(float sample);
    float get();
    
    
private:
    bool calculatePitch;
    float calculatedPitch;
    int sampleRate;
    int sampleIndex;
    const int fftOrder = 11;
    const int inputBufSize = 2048;
    juce::dsp::FFT fft;
    AudioBuffer<float> fftData;
    
};
