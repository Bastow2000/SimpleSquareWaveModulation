/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SModulatorAudioProcessor::SModulatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),tree(*this, nullptr, "PARAMETERS",createParameterLayout())
#endif
{
}

SModulatorAudioProcessor::~SModulatorAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SModulatorAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Make sure to update the number of reservations after adding params
    params.reserve(2);
    auto mix = std::make_unique<juce::AudioParameterFloat>(mixId, mixName,juce::NormalisableRange<float>(0.0f, 1.0f,0.1f),0.1f);
    auto frequency = std::make_unique<juce::AudioParameterFloat>(frequencyId, frequencyName,juce::NormalisableRange<float>(0.f, 1.f,0.1f),0.00f);
    
    params.push_back(std::move(mix));
    params.push_back(std::move(frequency));
    return{ params.begin(), params.end()};}

//==============================================================================
const juce::String SModulatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SModulatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SModulatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SModulatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SModulatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SModulatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SModulatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SModulatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SModulatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SModulatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SModulatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    process.reset(sampleRate, getTotalNumInputChannels());
}

void SModulatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SModulatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SModulatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    process.process(buffer);
    auto* mix = tree.getRawParameterValue(mixId);
    auto m = mix->load();
    auto* freq = tree.getRawParameterValue(frequencyId);
    auto f = freq->load();
    process.setFrequency(f);
    process.setMix(m);
}

//==============================================================================
bool SModulatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SModulatorAudioProcessor::createEditor()
{
    //return new SModulatorAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void SModulatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SModulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SModulatorAudioProcessor();
}
