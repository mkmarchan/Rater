/*
  ==============================================================================

    SyncGrain.cpp
    Created: 25 May 2019 5:55:03pm
    Author:  Mick Marchan

  ==============================================================================
*/

#include "SyncGrain.h"

SyncGrain::SyncGrain(int sampleRate, float grainRate, float grainFreq, float waveRatio) :
grainRate(grainRate),
grainFreq(grainFreq),
waveRatio(waveRatio),
sampleRate(sampleRate)
{
    grains = new grain[maxNumGrains];
    input = DelayBuffer(maxBufLen, 1);
    timeSinceLastTrigger = 0;
    int grainLen = maxBufLen;
    hann.initialise([grainLen](size_t i) {return (1.0 - cos(2 * M_PI * i / (grainLen - 1))) * 0.5;}, maxBufLen);
}

SyncGrain& SyncGrain::operator=(SyncGrain const &sG2) {
    this->grainRate = sG2.grainRate;
    this->grainFreq = sG2.grainFreq;
    this->waveRatio = sG2.waveRatio;
    this->sampleRate = sG2.sampleRate;
    
    grains = new grain[maxNumGrains];
    input = DelayBuffer(maxBufLen, 1);
    timeSinceLastTrigger = 0;
    int grainLen = maxBufLen;
    hann.initialise([grainLen](size_t i) {return (1.0 - cos(2 * M_PI * i / (grainLen - 1))) * 0.5;}, maxBufLen);
    
    return *this;
}

SyncGrain::SyncGrain() { }

SyncGrain::~SyncGrain() {
    delete(grains);
}

//==============================================================================
void SyncGrain::setParams(float grainRate, float grainFreq, float waveRatio) {
    this->grainRate = grainRate;
    this->grainFreq = grainFreq;
    this->waveRatio = waveRatio;
}
void SyncGrain::put(float sample) noexcept {
    input.put(0 , sample);
    float timeDiff = 1.0 / sampleRate;
    timeSinceLastTrigger += timeDiff;
    for (int i = 0; i < maxNumGrains; i++) {
        if (grains[i].grainFreq != 0 && grains[i].timePassed < 1.0 / grains[i].grainFreq) {
            grains[i].timePassed += timeDiff;
            grains[i].grainSampleOffset -= (waveRatio - 1.0) * timeDiff;
        }
    }

    if (timeSinceLastTrigger > 1.0 / grainRate) {
        for (int i = 0; i < maxNumGrains; i++) {
            if (grains[i].grainFreq == 0 || grains[i].timePassed > 1.0 / grains[i].grainFreq) {
                grains[i].timePassed = 0;
                grains[i].grainFreq = grainFreq;
                grains[i].waveRatio = waveRatio;
                grains[i].grainSampleOffset = waveRatio > 1.0 ? (waveRatio - 1.0) / grainFreq : 0;
                timeSinceLastTrigger = 0;
                break;
            }
        }
    }
}
float SyncGrain::get() noexcept  {
    float output = 0;

    for (int i = 0; i < maxNumGrains; i++) {
        if (grains[i].grainFreq != 0 && grains[i].timePassed < 1.0 / grains[i].grainFreq) {
            output += input.getOffset(0, grains[i].grainSampleOffset * sampleRate) * hann.getUnchecked(maxBufLen * grains[i].grainFreq * grains[i].timePassed);
        }
    }
    
    return output;
}
