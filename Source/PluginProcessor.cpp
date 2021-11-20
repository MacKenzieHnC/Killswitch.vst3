/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KillswitchAudioProcessor::KillswitchAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter (killswitchOn = new juce::AudioParameterBool ("killswitchOn",
                                                                "Killswitch",
                                                                false));
    justToggled = false;
    lastMode = false;
}

KillswitchAudioProcessor::~KillswitchAudioProcessor()
{
}

//==============================================================================
const juce::String KillswitchAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KillswitchAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KillswitchAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KillswitchAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KillswitchAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KillswitchAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KillswitchAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KillswitchAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KillswitchAudioProcessor::getProgramName (int index)
{
    return {};
}

void KillswitchAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KillswitchAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void KillswitchAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KillswitchAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KillswitchAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Split the incoming midi
    juce::MidiBuffer noteOn;
    juce::MidiBuffer noteOff;
    juce::MidiBuffer effects;
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        auto time = metadata.samplePosition;

        if (message.isNoteOn())
            noteOn.addEvent(juce::MidiMessage(message), time);
        else if (message.isNoteOff())
            noteOff.addEvent(juce::MidiMessage(message), time);
        else
            effects.addEvent(juce::MidiMessage(message), time);
    }
    midiMessages.clear(); // midiMessages acts as a temp buffer from here on out

    // Remove note-off from currentlyPlaying
    for (const auto metadata : currentlyPlaying)
    {
        bool found = false;
        for (const auto off : noteOff)
        {
            if (off.getMessage().getNoteNumber() == metadata.getMessage().getNoteNumber() && off.getMessage().getChannel() == metadata.getMessage().getChannel())
            {
                found = true;
                break;
            }
        }
        if (!found)
            midiMessages.addEvent(juce::MidiMessage(metadata.getMessage()), metadata.samplePosition);
    }
    currentlyPlaying.swapWith(midiMessages);
    midiMessages.clear();

    // Note offs always pass through
    for (const auto off : noteOff)
        midiMessages.addEvent(juce::MidiMessage(off.getMessage()), off.samplePosition);

    // Effects always pass through (for now)
    for (const auto effect : effects)
        midiMessages.addEvent(juce::MidiMessage(effect.getMessage()), effect.samplePosition);
    

    // If killswitch is on
    if (killswitchOn->AudioParameterBool::get())
    {
        // Kill audio
        buffer.clear();
        
        // Check if freshly toggled
        if (!lastMode)
        {
            lastMode = true;
            justToggled = true;
        }
        else
            justToggled = false;

        // Silence all currently audible notes
        if (justToggled)
            for (const auto on : currentlyPlaying)
                midiMessages.addEvent(juce::MidiMessage::noteOff(on.getMessage().getChannel(), on.getMessage().getNoteNumber()), on.samplePosition+1);
    } 
    // If killswitch is off
    else
    {
        // Check if freshly toggled
        if (lastMode)
        {
            lastMode = false;
            justToggled = true;
        }
        else
            justToggled = false;

        // if freshly toggled, pass through old notes
        if (justToggled)
            for (const auto on : currentlyPlaying)
                midiMessages.addEvent(juce::MidiMessage(on.getMessage()), on.samplePosition);

        // Pass through new notes
        for (const auto on : noteOn)
            midiMessages.addEvent(juce::MidiMessage(on.getMessage()), on.samplePosition);
    }

    // Add new notes to currentlyPlaying
    for (const auto on : noteOn)
        currentlyPlaying.addEvent(juce::MidiMessage(on.getMessage()), on.samplePosition);

     // switch midiMessages
    midiMessages.swapWith(midiMessages);
}

//==============================================================================
bool KillswitchAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* KillswitchAudioProcessor::createEditor()
{
    return new KillswitchAudioProcessorEditor (*this);
}

//==============================================================================
void KillswitchAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KillswitchAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KillswitchAudioProcessor();
}
