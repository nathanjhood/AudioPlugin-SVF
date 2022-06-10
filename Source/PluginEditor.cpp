/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p, APVTS& apvts, juce::UndoManager& um)
    :
    juce::AudioProcessorEditor(&p),
    audioProcessor(p),
    state(apvts),
    undoManager(um),
    subComponents(p, apvts),
    undoButton("Undo"),
    redoButton("Redo")
{
    setSize(350, 300);
    addAndMakeVisible(subComponents);
    addAndMakeVisible(undoButton);
    addAndMakeVisible(redoButton);
    undoButton.onClick = [this] { undoManager.undo(); };
    redoButton.onClick = [this] { undoManager.redo(); };
    setResizable(true, true);

    startTimerHz(24);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::timerCallback()
{
    undoManager.beginNewTransaction();
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::antiquewhite);

    //// draw an outline around the component
    /*g.setColour(juce::Colours::orangered);
    g.drawRect(getLocalBounds(), 2);*/

    // Add project info text to background here
    g.setColour(juce::Colours::black);
    g.setFont(15.0f);
    g.drawFittedText(ProjectInfo::companyName, getLocalBounds(), juce::Justification::topLeft, 1);
    g.drawFittedText(ProjectInfo::projectName, getLocalBounds(), juce::Justification::topRight, 1);
    g.drawFittedText(ProjectInfo::versionString, getLocalBounds(), juce::Justification::bottomLeft, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    subComponents.resized();
    subComponents.setBounds(0, 0, getWidth(), getHeight());
}
