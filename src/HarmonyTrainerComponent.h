// -*-c++-*-
#pragma once
#include <JuceHeader.h>

class HarmonyTrainerComponent : public juce::Component,
                                public juce::MidiInputCallback,
                                public juce::MidiKeyboardStateListener
{
public:
    HarmonyTrainerComponent();
    ~HarmonyTrainerComponent() override { }

    void setMidiInput( int mi );

    void resized() override;

    // MIDI -> me
    void handleIncomingMidiMessage( juce::MidiInput *source, const juce::MidiMessage &msg) override;

    // Keyboard UI -> me
    void handleNoteOn (juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
private:
    juce::AudioDeviceManager deviceManager;
    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;

    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HarmonyTrainerComponent);
};
