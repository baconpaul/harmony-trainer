#include "HarmonyTrainerComponent.h"
#include "AvailableAnalytics.h"
#include <iostream>
#include <iomanip>


HarmonyTrainerComponent::HarmonyTrainerComponent() :
    notesOn( 0 ), lastNote( 0 )
{
    keyboardComponent = std::make_unique<juce::MidiKeyboardComponent>( keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard ),
    keyboardState.addListener(this);
    addAndMakeVisible(keyboardComponent.get());

    // Open all midi devices
    auto list = juce::MidiInput::getAvailableDevices();
    for (auto device : list)
    {
        if (! deviceManager.isMidiInputDeviceEnabled (device.identifier))
        {
            deviceManager.setMidiInputDeviceEnabled (device.identifier, true);
            deviceManager.addMidiInputDeviceCallback (device.identifier, this);
        }
    }
    analytic = std::make_unique<ShowStuffAnalytic>();
    addAndMakeVisible(analytic.get());
}

void HarmonyTrainerComponent::resized()
{
    auto area = getLocalBounds();
    keyboardComponent->setBounds (area.removeFromTop (82).reduced(8));
    analytic->setBounds( area.withTop(82).reduced(8));

}

void HarmonyTrainerComponent::handleIncomingMidiMessage( juce::MidiInput *source, const juce::MidiMessage &msg)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent( msg );
}

void HarmonyTrainerComponent::handleNoteOn( juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    if( analytic != nullptr )
        analytic->noteOn( midiNoteNumber, velocity );
    repaint();
}

void HarmonyTrainerComponent::handleNoteOff( juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    if( analytic != nullptr )
        analytic->noteOff( midiNoteNumber, velocity );
    repaint();
}

void HarmonyTrainerComponent::paint( juce::Graphics &g )
{
}
