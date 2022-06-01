/*
  ==============================================================================

    PluginParameters.cpp
    Created: 29 May 2022 7:58:00pm
    Author:  Nathan J. Hood

  ==============================================================================
*/

#include "PluginParameters.h"
#include "PluginProcessor.h"

Parameters::Parameters(AudioPluginAudioProcessor& p, APVTS& apvts) : audioProcessor (p)
{
    frequencyPtr = static_cast         <juce::AudioParameterFloat*>        (apvts.getParameter("frequencyID"));
    jassert(frequencyPtr != nullptr);

    resonancePtr = static_cast         <juce::AudioParameterFloat*>        (apvts.getParameter("resonanceID"));
    jassert(resonancePtr != nullptr);

    typePtr = static_cast              <juce::AudioParameterChoice*>       (apvts.getParameter("typeID"));
    jassert(typePtr != nullptr);

    osPtr = static_cast                <juce::AudioParameterChoice*>       (apvts.getParameter("osID"));
    jassert(osPtr != nullptr);

    outputPtr = static_cast            <juce::AudioParameterFloat*>        (apvts.getParameter("outputID"));
    jassert(outputPtr != nullptr);

    mixPtr = static_cast              <juce::AudioParameterFloat*>        (apvts.getParameter("mixID"));
    jassert(mixPtr != nullptr);
}

void Parameters::setParameterLayout(Params& params)
{
    const auto dBMax = juce::Decibels::gainToDecibels(16.0f);
    const auto dBMin = juce::Decibels::gainToDecibels(0.5f, -120.0f) * 20.0f;

    auto freqRange = juce::NormalisableRange<float>(20.00f, 20000.00f, 0.01f, 00.198894f);
    auto gainRange = juce::NormalisableRange<float>(dBMin, dBMax, 0.01f, 1.00f);
    const auto mixRange = juce::NormalisableRange<float>(00.00f, 100.00f, 0.01f, 1.00f);

    auto fString = juce::StringArray({ "LP2",
    "LP1",
    "LP2n",
    "HP2",
    "HP1",
    "HP2n",
    "BP2",
    "BP2n",
    "AP2",
    "N2",
    "P2"
    });

    auto osString = juce::StringArray({ "1x", "2x", "4x", "8x", "16x" });

    params.add(std::make_unique<juce::AudioParameterFloat>("frequencyID", "Frequency", freqRange, 632.45f));
    params.add(std::make_unique<juce::AudioParameterFloat>("resonanceID", "Resonance", 00.00f, 01.00f, 00.10f));
    params.add(std::make_unique<juce::AudioParameterChoice>("typeID", "Type", fString, 0));
    params.add(std::make_unique<juce::AudioParameterChoice>("osID", "Oversampling", osString, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("outputID", "Output", gainRange, 00.00f));
    params.add(std::make_unique<juce::AudioParameterFloat>("mixID", "Mix", mixRange, 100.00f));
}