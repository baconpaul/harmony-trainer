#include <JuceHeader.h>
#include "HarmonyTrainerComponent.h"

//==============================================================================
class HarmonyTrainerApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    HarmonyTrainerApplication() {}

    ~HarmonyTrainerApplication() override
    {
    }

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return false; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        mainWindow.reset (new HarmonyTrainerWindow (getApplicationName()));
    }

    void shutdown() override             { mainWindow = nullptr; }

    //==============================================================================
    void systemRequestedQuit() override                                 { quit(); }

private:
    class HarmonyTrainerWindow    : public juce::DocumentWindow
    {
    public:
        HarmonyTrainerWindow (const juce::String& name)
            : juce::DocumentWindow (name,
                                    juce::Desktop::getInstance().getDefaultLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId),
                                    juce::DocumentWindow::allButtons)
            {
                setUsingNativeTitleBar (true);
                setResizable (true, false);
                setResizeLimits (400, 400, 10000, 10000);

                auto bx = (int) (0.1f * getParentWidth()),
                    by =(int) (0.1f * getParentHeight()),
                    bw = juce::jmax (850, (int) (0.5f * getParentWidth())),
                    bh = juce::jmax (600, (int) (0.7f * getParentHeight()));

#if ! JUCE_MAC
                bh += juce::LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight();
#endif
                    
                setBounds( bx, by, bw, bh );

                setContentOwned (new HarmonyTrainerComponent(), false);
                setVisible (true);
            }

        void closeButtonPressed() override    { juce::JUCEApplication::getInstance()->systemRequestedQuit(); }

        //==============================================================================
        HarmonyTrainerComponent& getHarmonyTrainerComponent()    { return *dynamic_cast<HarmonyTrainerComponent*> (getContentComponent()); }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonyTrainerWindow)
    };

    std::unique_ptr<HarmonyTrainerWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (HarmonyTrainerApplication)
