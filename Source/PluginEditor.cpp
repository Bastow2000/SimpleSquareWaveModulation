/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SModulatorAudioProcessorEditor::SModulatorAudioProcessorEditor (SModulatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    mixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
// mixSlider.setValue(process.mix);

   mixSlider.addListener(this);
   freqSlider.addListener(this);
   addAndMakeVisible(mixSlider);
   addAndMakeVisible(freqSlider);

  mixVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, mixId, mixSlider);
  freqVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, frequencyId, freqSlider);
  
    setSize (400, 300);
}

SModulatorAudioProcessorEditor::~SModulatorAudioProcessorEditor()
{
}

//==============================================================================
void SModulatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SModulatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mixSlider.setBounds(10, 10, getWidth() , 260);
    freqSlider.setBounds(10, 10, getWidth() , 150);
}

  void SModulatorAudioProcessorEditor::sliderValueChanged(juce::Slider* /*slider*/) 
  {

  }