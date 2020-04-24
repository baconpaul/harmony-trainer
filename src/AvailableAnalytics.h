// -*-c++-*-

#pragma once

#include <JuceHeader.h>
#include "PracticeAnalytic.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

class TimedRecordingPracticeAnalytic : public PracticeAnalytic {
public:
    typedef std::chrono::steady_clock clk_t;
    struct RecordedNote {
        int note;
        float velocity;
        std::chrono::time_point<clk_t> noteAt;
        std::chrono::duration<double> heldFor;
    };

    std::array<RecordedNote, 128> openNotes;
    std::vector<RecordedNote> pastNotes;

    virtual void noteOn( int noteNumber, float velocity ) override
    {
        RecordedNote n;
        n.note = noteNumber;
        n.velocity = velocity;
        n.noteAt = clk_t::now();
        openNotes[noteNumber] = n;
    }
    
    virtual void noteOff( int noteNumber, float velocity ) override
    {
        RecordedNote n = openNotes[noteNumber];
        n.heldFor = clk_t::now() - n.noteAt;
        pastNotes.push_back( n );
    }
};

class TestAnalytic : public TimedRecordingPracticeAnalytic {
public:
    virtual void paint( juce::Graphics &g ) override {
        auto region = getLocalBounds();
        g.setColour( juce::Colour( 255 - ( pastNotes.size() * 5 ) % 255, 0, ( pastNotes.size() * 5 ) % 255 ) );
        g.fillRect( region );

    }
};

class VelocityAndDurationHistogram : public PracticeAnalytic {
public:
    typedef std::chrono::steady_clock clk_t;

    static const int velocitybuckets = 100;
    
    std::array<std::chrono::time_point<clk_t>, 128> openNotes;
    std::array<int, velocitybuckets> velocityhisto;

    VelocityAndDurationHistogram() {
        for( auto i=0; i<velocitybuckets; ++i )
            velocityhisto[i] = 0;
    }
    
    virtual void noteOn( int noteNumber, float velocity ) override
    {
        auto bv = (int)( velocity * velocitybuckets );
        velocityhisto[bv] ++;
        openNotes[noteNumber] = clk_t::now();
    }
    
    virtual void noteOff( int noteNumber, float velocity ) override
    {
    }

    virtual void paint( juce::Graphics &g ) override
    {
        auto region = getLocalBounds();

        auto y0 = region.getHeight() / 2;
        auto me = std::max_element( velocityhisto.begin(), velocityhisto.end() );
        auto mv = *me;

        g.setColour( juce::Colour( 255, 255, 255 ) );
        for( int i=0; i<velocitybuckets; ++i )
        {
            int xp = i * 2;
            g.fillRect( xp, y0, xp  + 1, - velocityhisto[i] * mv );
        }
    }
};


