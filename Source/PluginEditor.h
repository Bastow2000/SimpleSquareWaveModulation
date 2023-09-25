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
class SModulatorAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    SModulatorAudioProcessorEditor (SModulatorAudioProcessor&);
    ~SModulatorAudioProcessorEditor() override;
     void sliderValueChanged(juce::Slider* slider) override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SModulatorAudioProcessor& audioProcessor;

    juce::Slider mixSlider, freqSlider;
 
    SquareWaveMix process;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> mixVal;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> freqVal;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SModulatorAudioProcessorEditor)
};
