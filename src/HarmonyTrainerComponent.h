// -*-c++-*-
#pragma once
#include <JuceHeader.h>
#include "PracticeAnalytic.h"
#include <memory>

class HarmonyTrainerComponent : public juce::Component,
                                public juce::MidiInputCallback,
                                public juce::MidiKeyboardStateListener
{
public:
    HarmonyTrainerComponent();
    ~HarmonyTrainerComponent() override { }

    void resized() override;

    // MIDI -> me
    void handleIncomingMidiMessage( juce::MidiInput *source, const juce::MidiMessage &msg) override;

    // Keyboard UI -> me
    void handleNoteOn (juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

    void paint( juce::Graphics & g ) override;
    
private:
    juce::AudioDeviceManager deviceManager;

    juce::MidiKeyboardState keyboardState;
    std::unique_ptr<juce::MidiKeyboardComponent> keyboardComponent;
    std::unique_ptr<PracticeAnalytic> analytic;
    
    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;

    std::atomic<int> lastNote;
    std::atomic<int> notesOn;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HarmonyTrainerComponent);
};
