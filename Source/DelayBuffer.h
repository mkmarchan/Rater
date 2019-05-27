//
//  DelayBuffer.h
//  Rater
//
//  Created by Mick Marchan on 8/1/18.
//

#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 */
class DelayBuffer
{
public:
    DelayBuffer();
    DelayBuffer(int maxBufLen, int numChannels);
    ~DelayBuffer();
    
    //==============================================================================
    void put(int channel, float sample);
    float getOffset(int channel, float delay);
    
private:
    int maxBufLen, numChannels;
    std::vector<int> channelCounters;
    AudioBuffer<float> buffer;
    
};
