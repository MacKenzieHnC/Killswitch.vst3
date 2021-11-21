/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KillswitchAudioProcessorEditor::KillswitchAudioProcessorEditor (KillswitchAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setResizable(true,false);

    auto sidebarMinHeight = (numOptions * (sideItemHeight + sideItemMargin)) + sideItemMargin;

    setResizeLimits(sidebarMinWidth + killswitchMinSide + (sideItemMargin*3), juce::jmax(sidebarMinHeight, killswitchMinSide + (sideItemMargin * 2)), 10000, 10000);

    sidebar.setColour(juce::Label::backgroundColourId, sidebarBGColour);
    addAndMakeVisible(sidebar);

    resumeNotesButton.setColour(juce::ToggleButton::tickColourId, textColour);
    resumeNotesButton.setColour(juce::ToggleButton::tickDisabledColourId, textColour);
    addAndMakeVisible(resumeNotesButton);
    resumeNotesButton.addListener(this);

    resumeNotesLabel.setText("Resume withheld MIDI notes", juce::dontSendNotification);
    resumeNotesLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(resumeNotesLabel);
    resumeNotesConnector.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.options, "RESUME_NOTES", resumeNotesButton));

    blockNonNotesButton.setColour(juce::ToggleButton::tickColourId, textColour);
    blockNonNotesButton.setColour(juce::ToggleButton::tickDisabledColourId, textColour);
    addAndMakeVisible(blockNonNotesButton);
    blockNonNotesButton.addListener(this);
    blockNonNotesConnector.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.options, "BLOCK_NON_NOTES", blockNonNotesButton));

    midiOptionsLabel.setText("MIDI Options", juce::dontSendNotification);
    midiOptionsLabel.setColour(juce::Label::textColourId, textColour);
    midiOptionsLabel.setColour(juce::Label::backgroundColourId, optionsLabelBGColour);
    addAndMakeVisible(midiOptionsLabel);

    blockNonNotesLabel.setText("Block non-note messages", juce::dontSendNotification);
    blockNonNotesLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(blockNonNotesLabel);

    killswitch.setColour(juce::TextButton::buttonColourId, buttonColour);
    killswitch.setButtonText("Kill");
    addAndMakeVisible (killswitch);
    killswitch.addListener(this);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

KillswitchAudioProcessorEditor::~KillswitchAudioProcessorEditor()
{
}

//==============================================================================
void KillswitchAudioProcessorEditor::paint (juce::Graphics& g)
{   
    // fill the whole window white
    g.fillAll (bgColour);
}

void KillswitchAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    auto sidebarArea = area.removeFromLeft(juce::jmax(sidebarMinWidth, area.getWidth() / 3));
    sidebar.setBounds(sidebarArea);

    auto subSidebarArea = sidebarArea.removeFromTop(sideItemHeight);
    midiOptionsLabel.setBounds(subSidebarArea.removeFromTop(sideItemHeight));

    subSidebarArea = sidebarArea.removeFromTop(sideItemHeight);
    resumeNotesButton.setBounds(subSidebarArea.removeFromLeft(sideItemHeight).reduced(sideItemMargin));
    resumeNotesLabel.setBounds(subSidebarArea);

    subSidebarArea = sidebarArea.removeFromTop(sideItemHeight);
    blockNonNotesButton.setBounds(subSidebarArea.removeFromLeft(sideItemHeight).reduced(sideItemMargin));
    blockNonNotesLabel.setBounds(subSidebarArea);

    auto killswitchWidth = juce::jmax(100, area.getWidth() / 4);
    auto killswitchHeight = juce::jmax(100, area.getHeight() / 4);
    killswitch.setBounds(area.getCentreX() - (killswitchWidth/2), area.getCentreY() - (killswitchHeight/2), killswitchWidth, killswitchHeight);
}

void KillswitchAudioProcessorEditor::buttonClicked (juce::Button* button)
{
}

void KillswitchAudioProcessorEditor::buttonStateChanged (juce::Button* button)
{
    if (button == &killswitch)
        if (button->isDown())
            audioProcessor.killswitchOn->AudioParameterBool::operator=(true);
        else
            audioProcessor.killswitchOn->AudioParameterBool::operator=(false);
}