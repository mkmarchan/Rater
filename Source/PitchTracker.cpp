/*
  ==============================================================================

    PitchTracker.cpp
    Created: 26 May 2019 3:24:43pm
    Author:  Mick Marchan

  ==============================================================================
*/

#include "PitchTracker.h"


PitchTracker::PitchTracker(int sampleRate) :
calculatePitch(false),
calculatedPitch(0),
sampleRate(sampleRate),
sampleIndex(0),
fft(fftOrder),
fftData(1, 2 * inputBufSize)
{
}

PitchTracker::~PitchTracker() {
}

void PitchTracker::put(float sample) {
    if (sampleIndex == 0) {
        fftData.clear();
    }
    
    fftData.setSample(0, sampleIndex, sample);
    sampleIndex = (sampleIndex + 1) % inputBufSize;
    
    if (sampleIndex == 0) {
        calculatePitch = true;
    }
}

float PitchTracker::get() {
    if (calculatePitch) {
        float* writePtr = fftData.getWritePointer(0);
        fft.performFrequencyOnlyForwardTransform(writePtr);
        
        int maxAmplitudeIndex = 0;
        for (int i = 0; i < inputBufSize; i++) {
            if (writePtr[i] > writePtr[maxAmplitudeIndex]) {
                maxAmplitudeIndex = i;
            }
        }
        calculatedPitch = maxAmplitudeIndex * sampleRate / inputBufSize;
        calculatePitch = false;
    }
    
    return calculatedPitch;
}
