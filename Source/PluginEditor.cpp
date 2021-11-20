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
    killswitch.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
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
    g.fillAll (juce::Colours::white);
}

void KillswitchAudioProcessorEditor::resized()
{
    killswitch.centreWithSize(100,100);
}

void KillswitchAudioProcessorEditor::buttonClicked (juce::Button* button)
{
}

void KillswitchAudioProcessorEditor::buttonStateChanged (juce::Button* button)
{
    if(button == &killswitch)
        if(button->isDown())
            audioProcessor.killswitchOn->AudioParameterBool::operator=(true);
        else
            audioProcessor.killswitchOn->AudioParameterBool::operator=(false);
}
