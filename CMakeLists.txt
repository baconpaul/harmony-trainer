cmake_minimum_required(VERSION 3.10)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11 CACHE STRING "Build for 10.11")

project(HarmonyTrainer VERSION 0.9.0 LANGUAGES CXX)

add_subdirectory(JUCE)

juce_add_gui_app(HarmonyTrainer
    BUNDLE_ID                       org.baconpaul.HarmonyTrainer
    COMPANY_NAME                    "BaconPaul Stuff"
    COMPANY_WEBSITE                 "https://www.baconpaul.org/"
    # ICON_BIG                        ${CMAKE_CURRENT_SOURCE_DIR}/Source/JUCEAppIcon.png
    NEEDS_CURL                      FALSE
    NEEDS_WEB_BROWSER               FALSE
)

juce_generate_juce_header(HarmonyTrainer)

target_sources(HarmonyTrainer PRIVATE
    src/Main.cpp
    src/HarmonyTrainerComponent.cpp
)

target_compile_definitions(HarmonyTrainer PRIVATE
    JUCE_ALLOW_STATIC_NULL_VARIABLES=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1
    DONT_SET_USING_JUCE_NAMESPACE=1
)

target_link_libraries(HarmonyTrainer PRIVATE
    juce::juce_analytics
    juce::juce_audio_utils
    juce::juce_blocks_basics
    juce::juce_box2d
    juce::juce_dsp
)

