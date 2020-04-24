// -*-c++-*-
#pragma once
#include <JuceHeader.h>
#include "PracticeAnalytic.h"
#include <memory>

class HarmonyTrainerComponent : public juce::Component,
                                public juce::MidiInputCallback,
                                public juce::MidiKeyboardStateListener,
                                public juce::MenuBarModel
{
public:
    HarmonyTrainerComponent();
    ~HarmonyTrainerComponent() override;

    void resized() override;

    // MIDI -> me
    void handleIncomingMidiMessage( juce::MidiInput *source, const juce::MidiMessage &msg) override;

    // Keyboard UI -> me
    void handleNoteOn (juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

    void paint( juce::Graphics & g ) override;

    virtual juce::StringArray getMenuBarNames() override {
        juce::StringArray res;
        res.add("File");
        res.add("Modes");
        res.add("Help");
        return res;
    }

    virtual juce::PopupMenu getMenuForIndex( int idx, const juce::String &n ) override;
    virtual void menuItemSelected( int id, int topLevelMenuIndex ) override;

    void replaceAnalytic( std::shared_ptr<PracticeAnalytic> na );
private:
    juce::AudioDeviceManager deviceManager;

    juce::MidiKeyboardState keyboardState;
    std::shared_ptr<juce::MidiKeyboardComponent> keyboardComponent;
    std::shared_ptr<PracticeAnalytic> analytic;
    
    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;

    std::atomic<int> lastNote;
    std::atomic<int> notesOn;

#if ! JUCE_MAC
    std::shared_ptr<juce::MenuBarComponent> menuBar;
#endif    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HarmonyTrainerComponent);
};
