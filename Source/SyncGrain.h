/*
  ==============================================================================

    SyncGrain.h
    Created: 25 May 2019 5:55:03pm
    Author:  Mick Marchan

  ==============================================================================
*/

#pragma once
#include "DelayBuffer.h"

typedef struct grain {
    float timePassed;
    float grainSampleOffset;
    float grainFreq;
    float waveRatio;
} grain;

class SyncGrain
{
public:
    SyncGrain();
    SyncGrain(int sampleRate, float grainRate, float grainFreq, float waveRatio);
    ~SyncGrain();
    
    //==============================================================================
    void setParams(float grainRate, float grainFreq, float waveRatio);
    void  put(float sample) noexcept;
    float get() noexcept;
    
    SyncGrain& operator=(SyncGrain const &sG2);
    
private:
    float grainRate;
    float grainFreq;
    float waveRatio;
    float timeSinceLastTrigger;
    
    DelayBuffer input;
    grain* grains;
    
    int sampleRate;
    const int maxBufLen = 44100;
    const int maxNumGrains = 10;
    
    juce::dsp::LookupTable<float> hann;
};
