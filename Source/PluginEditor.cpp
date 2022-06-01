/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), audioProcessor(p), state(p.getAPVTS()), subComponents(p, p.getAPVTS())
{
    setSize(400, 300);
    addAndMakeVisible(subComponents);
    setSize(subComponents.getWidth() * 1.333, subComponents.getHeight() * 1.333);
    setResizable(true, false);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("StoneyDSP", getLocalBounds(), juce::Justification::topLeft, 1);
    g.drawFittedText(audioProcessor.getName(), getLocalBounds(), juce::Justification::topRight, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    subComponents.setBounds(0, getHeight() - 100, getWidth(), 100);
    subComponents.resized();
}
