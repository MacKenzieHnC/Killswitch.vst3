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
    void KillswitchAudioProcessorEditor::buttonStateChanged (juce::Button* button) override;
    KillswitchAudioProcessorEditor (KillswitchAudioProcessor&);
    ~KillswitchAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton killswitch;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KillswitchAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KillswitchAudioProcessorEditor)
};
