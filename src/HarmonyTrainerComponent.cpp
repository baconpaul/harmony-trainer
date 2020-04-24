#include "HarmonyTrainerComponent.h"
#include "AvailableAnalytics.h"
#include <iostream>
#include <iomanip>


HarmonyTrainerComponent::HarmonyTrainerComponent() :
    notesOn( 0 ), lastNote( 0 )
{
    keyboardComponent = std::make_shared<juce::MidiKeyboardComponent>( keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard ),
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

    // TODO - make it so you can save your selected analytic
    analytic = std::make_shared<TestAnalytic>();
    addAndMakeVisible(analytic.get());

    
#if JUCE_MAC
    MenuBarModel::setMacMainMenu (this);
#else
    menuBar = std::make_shared<juce::MenuBarComponent>(this);
    addAndMakeVisible(menuBar.get());
#endif

}

HarmonyTrainerComponent::~HarmonyTrainerComponent()
{
#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif

}

void HarmonyTrainerComponent::resized()
{
    auto area = getLocalBounds();
#if ! JUCE_MAC
    menuBar->setBounds (area.removeFromTop (juce::LookAndFeel::getDefaultLookAndFeel()
                                            .getDefaultMenuBarHeight()));

    area.setTop( juce::LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight() );
#endif
    
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

void HarmonyTrainerComponent::replaceAnalytic( std::shared_ptr<PracticeAnalytic> n )
{
    if( analytic.get() )
        removeChildComponent( analytic.get() );
    analytic = n;
    addAndMakeVisible( analytic.get() );
    resized();
    repaint();
}

juce::PopupMenu HarmonyTrainerComponent::getMenuForIndex( int idx, const juce::String &s )
{
    std::cout << "GEtting " << idx << " " << s << std::endl;
    auto res = juce::PopupMenu();
    if( idx == 0 ) // file
    {
        res.addItem( "Export", []() { std::cout << "Export that Puppy" << std::endl; } );
    }
    else if (idx == 1 ) // modes
    {
        res.addItem( "Velocity and Duration Histogram", [this]() { this->replaceAnalytic( std::make_shared<VelocityAndDurationHistogram>() ); } );
        res.addItem( "Test Analytic", [this]() { this->replaceAnalytic( std::make_shared<TestAnalytic>() ); } );
    }
    else if (idx == 2 ) // help
    {
        res.addItem( "WebSite", []() { std::cout << "Visit that Website" << std::endl; } );
    }
    return res;
}

void HarmonyTrainerComponent::menuItemSelected( int id, int tlm )
{
    std::cout << "Menu Selected " << id << " " << tlm << std::endl;
}
