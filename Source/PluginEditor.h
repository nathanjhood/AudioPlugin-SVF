/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "./Components/AutoComponent.h"

//==============================================================================
/**
*/
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    using APVTS = juce::AudioProcessorValueTreeState;

    //==========================================================================
    AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p);
    ~AudioPluginAudioProcessorEditor() override;

    //==========================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& audioProcessor;
    APVTS& state;

    AutoComponent subComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
