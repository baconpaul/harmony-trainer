// -*-c++-*-
#pragma once

#include <JuceHeader.h>
#include <chrono>

class PracticeAnalytic : public juce::Component {
public:
    virtual ~PracticeAnalytic() = default;

    virtual void reset() { }
    virtual void noteOn( int noteNumber, float velocity ) = 0;
    virtual void noteOff( int noteNumber, float velocity ) = 0;
};

