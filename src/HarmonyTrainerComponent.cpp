#include "HarmonyTrainerComponent.h"
#include "AvailableAnalytics.h"
#include <iostream>
#include <iomanip>


HarmonyTrainerComponent::HarmonyTrainerComponent() :
    keyboardComponent( keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard ),
    notesOn( 0 ), lastNote( 0 )
{
    analytic = std::make_unique<ShowStuffAnalytic>();
    
    keyboardState.addListener(this);
    
    auto list = juce::MidiInput::getAvailableDevices();
 
    // find the first enabled device and use that by default
    for (auto device : list)
    {
        std::cout << "device : " << device.name << std::endl;
        if (! deviceManager.isMidiInputDeviceEnabled (device.identifier))
        {
            std::cout << "Enabling device" << std::endl;
            deviceManager.setMidiInputDeviceEnabled (device.identifier, true);
            deviceManager.addMidiInputDeviceCallback (device.identifier, this);
        }
    }

    addAndMakeVisible(keyboardComponent);
}

void HarmonyTrainerComponent::resized()
{
    auto area = getLocalBounds();
    keyboardComponent.setBounds (area.removeFromTop (82).reduced(8));
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
    if( analytic != nullptr )
    {
        auto area = getLocalBounds().withTop( 82 ).reduced(8);
        
        analytic->paint( g, area );
    }
}
