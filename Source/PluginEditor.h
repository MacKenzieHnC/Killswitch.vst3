/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class KillswitchAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Button::Listener
{
public:
    void buttonClicked (juce::Button* button) override;
    void buttonStateChanged (juce::Button* button) override;
    KillswitchAudioProcessorEditor (KillswitchAudioProcessor&);
    ~KillswitchAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Buttons and labels
    juce::Label sidebar;
    juce::TextButton killswitch;
    juce::ToggleButton resumeNotesButton;
    juce::Label midiOptionsLabel;
    juce::Label resumeNotesLabel;
    juce::ToggleButton blockNonNotesButton;
    juce::Label blockNonNotesLabel;

    int numOptions = 3;

    // Connectors
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> resumeNotesConnector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> blockNonNotesConnector;

    // Colors
    juce::Colour bgColour = juce::Colour::fromString("#FF766174");
    juce::Colour optionsLabelBGColour = juce::Colour::fromString("#FF281C26");
    juce::Colour sidebarBGColour = juce::Colour::fromString("#FF543F52");
    juce::Colour buttonColour = optionsLabelBGColour;
    juce::Colour textColour = juce::Colour::fromString("#FFF0E8F3");

    // Component Sizes
    int sidebarMinWidth = 100;
    int sideItemHeight = 40;
    int sideItemMargin = 5;
    int killswitchMinSide = 100;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KillswitchAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KillswitchAudioProcessorEditor)
};
