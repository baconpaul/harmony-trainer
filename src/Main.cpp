#include <JuceHeader.h>
#include "HarmonyTrainerComponent.h"

//==============================================================================
class HarmonyTrainerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    HarmonyTrainerApplication() {}

    ~HarmonyTrainerApplication() override
    {
    }

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        mainWindow.reset (new HarmonyTrainerWindow (getApplicationName()));
    }

    void shutdown() override             { mainWindow = nullptr; }

    //==============================================================================
    void systemRequestedQuit() override                                 { quit(); }
    void anotherInstanceStarted (const String&) override                {}

private:
    class HarmonyTrainerWindow    : public DocumentWindow
    {
    public:
        HarmonyTrainerWindow (const String& name)
            : DocumentWindow (name,
                              Desktop::getInstance().getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
            {
                setUsingNativeTitleBar (true);
                setResizable (true, false);
                setResizeLimits (400, 400, 10000, 10000);

                setBounds ((int) (0.1f * getParentWidth()),
                           (int) (0.1f * getParentHeight()),
                           jmax (850, (int) (0.5f * getParentWidth())),
                           jmax (600, (int) (0.7f * getParentHeight())));

                setContentOwned (new HarmonyTrainerComponent(), false);
                setVisible (true);
            }

        void closeButtonPressed() override    { JUCEApplication::getInstance()->systemRequestedQuit(); }

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
