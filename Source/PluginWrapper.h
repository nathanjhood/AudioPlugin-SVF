/*
  ==============================================================================

    PluginWrapper.h
    Created: 8 May 2022 9:38:17pm
    Author:  StoneyDSP

  ==============================================================================
*/

#pragma once

#ifndef PLUGINWRAPPER_H_INCLUDED
#define PLUGINWRAPPER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Modules/SVF.h"

class AudioPluginSVFAudioProcessor;

template <typename SampleType>
class ProcessWrapper
{
public:
    using APVTS = juce::AudioProcessorValueTreeState;
    using Type = juce::dsp::StateVariableTPTFilterType;
    //==========================================================================
    /** Constructor. */
    ProcessWrapper(AudioPluginSVFAudioProcessor& p, APVTS& apvts, juce::dsp::ProcessSpec& spec);

    //==========================================================================
    /** Initialises the processor. */
    void prepare(juce::dsp::ProcessSpec& spec);

    /** Resets the internal state variables of the processor. */
    void reset();

    //==========================================================================
    /** Updates the internal state variables of the processor. */
    void update();

    //==========================================================================
    void process(juce::AudioBuffer<SampleType>& buffer, juce::MidiBuffer& midiMessages);

    SampleType getLatencySamples() const noexcept;

private:
    //==========================================================================
    // This reference is provided as a quick way for the wrapper to
    // access the processor object that created it.
    AudioPluginSVFAudioProcessor& audioProcessor;
    APVTS& state;
    juce::dsp::ProcessSpec& setup;

    //==========================================================================
    /** Sets the oversampling factor. */
    void setOversampling();

    //==========================================================================
    std::unique_ptr<juce::dsp::Oversampling<SampleType>> oversampler[5];

    //==========================================================================
    /** Instantiate objects. */
    juce::dsp::DryWetMixer<SampleType> mixer;
    StateVariableTPTFilter<SampleType> svf;
    juce::dsp::Gain<SampleType> output;

    //==========================================================================
    /** Parameter pointers. */
    juce::AudioParameterFloat* frequencyPtr { nullptr };
    juce::AudioParameterFloat* resonancePtr { nullptr };
    juce::AudioParameterChoice* typePtr { nullptr };
    juce::AudioParameterChoice* osPtr { nullptr };
    juce::AudioParameterFloat* outputPtr { nullptr };
    juce::AudioParameterFloat* mixPtr { nullptr };
    juce::AudioParameterBool* bypassPtr{ nullptr };

    //==========================================================================
    /** Init variables. */
    int curOS = 0, prevOS = 0, oversamplingFactor = 1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessWrapper)
};

#endif //  PLUGINWRAPPER_H_INCLUDED