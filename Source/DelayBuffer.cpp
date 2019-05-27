//
//  DelayBuffer.cpp
//  Rater - VST
//
//  Created by Mick Marchan on 8/1/18.
//

#include "DelayBuffer.h"

//==============================================================================
DelayBuffer::DelayBuffer (int maxBufLen, int numChannels)
: maxBufLen (maxBufLen), numChannels (numChannels)
{
    buffer.setSize(numChannels, maxBufLen);
    buffer.clear();
    for (int i = 0; i < numChannels; i++) {
        channelCounters.push_back(-1);
    }
}

DelayBuffer::DelayBuffer()
{
}

DelayBuffer::~DelayBuffer()
{
}

//==============================================================================
void DelayBuffer::put(int channel, float sample)
{
    channelCounters[channel] = (channelCounters[channel] + 1) % maxBufLen;
    buffer.setSample(channel, channelCounters[channel], sample);
}

float DelayBuffer::getOffset(int channel, float delay)
{
    // Error check
    if (delay < 0) {
        delay = 0;
    }
    
    // get buffer index
    float index = channelCounters[channel] - delay;
    while (index < 0) index += maxBufLen;
    
    // get weight
    float floorWeight = ceil(index) - index;
    
    return buffer.getSample(channel, floor(index)) * floorWeight
    + buffer.getSample(channel, ceil(index)) * (1 - floorWeight);
}

