// -*-c++-*-
#pragma once

#include <JuceHeader.h>
#include <chrono>

class PracticeAnalytic {
public:
    virtual ~PracticeAnalytic() = default;

    virtual void reset() { }
    virtual void noteOn( int noteNumber, float velocity ) = 0;
    virtual void noteOff( int noteNumber, float velocity ) = 0;

    virtual void paint( juce::Graphics &g, juce::Rectangle<int> region ) = 0;
};

