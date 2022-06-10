/*
  ==============================================================================

    PluginParameters.cpp
    Created: 29 May 2022 7:58:00pm
    Author:  Nathan J. Hood

  ==============================================================================
*/

#include "PluginParameters.h"
#include "PluginProcessor.h"

Parameters::Parameters(AudioPluginAudioProcessor& p, APVTS& apvts)
    :
    audioProcessor(p),
    state(apvts)
{
}

void Parameters::setParameterLayout(Params& params)
{
    const auto dBMax = juce::Decibels::gainToDecibels(16.0f);
    const auto dBOut = juce::Decibels::gainToDecibels(0.5f, -120.0f) * 20.0f;

    const auto freqRange = juce::NormalisableRange<float>(20.00f, 20000.00f, 0.01f, 00.198894f);
    const auto resRange = juce::NormalisableRange<float>(00.00f, 1.00f, 0.01f, 1.00f);
    const auto gainRange = juce::NormalisableRange<float>(dBOut, dBMax, 0.01f, 1.00f);
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

    const auto decibels = juce::String{ ("dB") };
    const auto frequency = juce::String{ ("Hz") };
    const auto reso = juce::String{ ("q") };
    const auto percentage = juce::String{ ("%") };

    auto genParam = juce::AudioProcessorParameter::genericParameter;
    auto inMeter = juce::AudioProcessorParameter::inputMeter;
    auto outParam = juce::AudioProcessorParameter::outputGain;
    auto outMeter = juce::AudioProcessorParameter::outputMeter;

    juce::ignoreUnused(inMeter);
    juce::ignoreUnused(outMeter);

    params.add(std::make_unique<juce::AudioParameterFloat>("frequencyID", "Frequency", freqRange, 632.45f, frequency, genParam));
    params.add(std::make_unique<juce::AudioParameterFloat>("resonanceID", "Resonance", resRange, 00.10f, reso, genParam));
    params.add(std::make_unique<juce::AudioParameterChoice>("typeID", "Type", fString, 0));
    params.add(std::make_unique<juce::AudioParameterChoice>("osID", "Oversampling", osString, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("outputID", "Output", gainRange, 00.00f, decibels, outParam));
    params.add(std::make_unique<juce::AudioParameterFloat>("mixID", "Mix", mixRange, 100.00f, percentage, genParam));
    params.add(std::make_unique<juce::AudioParameterBool>("bypassID", "Bypass", false));
}