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
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    using APVTS = juce::AudioProcessorValueTreeState;

    //==========================================================================
    AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p, APVTS& apvts);
    ~AudioPluginAudioProcessorEditor() override;

    //==========================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& audioProcessor;
    APVTS& state;

    juce::Slider outputSlider;
    juce::Slider dryWetSlider;
    juce::ToggleButton bypassButton;
    juce::ToggleButton displayButton;

    std::unique_ptr<APVTS::SliderAttachment> outputAttach;
    std::unique_ptr<APVTS::SliderAttachment> dryWetAttach;
    std::unique_ptr<APVTS::ButtonAttachment> bypassAttach;
    std::unique_ptr<APVTS::ButtonAttachment> displayAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
