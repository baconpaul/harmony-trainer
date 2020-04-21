#include "HarmonyTrainerComponent.h"
#include <iostream>
#include <iomanip>

HarmonyTrainerComponent::HarmonyTrainerComponent() :
    keyboardComponent( keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard )
{
    keyboardState.addListener(this);
    
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");

    auto midiInputs = juce::MidiInput::getDevices();
    midiInputList.addItemList (midiInputs, 1);
    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };
 
    // find the first enabled device and use that by default
    for (auto midiInput : midiInputs)
    {
        if (deviceManager.isMidiInputEnabled (midiInput))
        {
            setMidiInput (midiInputs.indexOf (midiInput));
            break;
        }
    }
 
    // if no enabled devices were found just use the first one in the list
    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);

    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };

    addAndMakeVisible(keyboardComponent);
}

void HarmonyTrainerComponent::resized()
{
    auto area = getLocalBounds();
    midiInputList    .setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
    keyboardComponent.setBounds (area.removeFromTop (80).reduced(8));
}

void HarmonyTrainerComponent::setMidiInput( int index )
{
    std::cout << "Midi Imput is now " << index << std::endl;

    auto list = juce::MidiInput::getAvailableDevices();

    if( lastInputIndex <= list.size() )
        deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);
    
    auto newInput = list[index];
    
    if (! deviceManager.isMidiInputDeviceEnabled (newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled (newInput.identifier, true);

    deviceManager.addMidiInputDeviceCallback (newInput.identifier, this);
    midiInputList.setSelectedId (index + 1, juce::dontSendNotification);

    lastInputIndex = index;
}

void HarmonyTrainerComponent::handleIncomingMidiMessage( juce::MidiInput *source, const juce::MidiMessage &msg)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
    std::cout << "Got a MIDI message" << std::endl;
    keyboardState.processNextMidiEvent( msg );
}

void HarmonyTrainerComponent::handleNoteOn( juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    if( ! isAddingFromMidiInput )
        std::cout << "UI Based Note On" << std::endl;
}

void HarmonyTrainerComponent::handleNoteOff( juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    if( ! isAddingFromMidiInput )
        std::cout << "UI Based Note Off" << std::endl;
}

