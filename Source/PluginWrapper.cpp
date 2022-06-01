/*
  ==============================================================================

    PluginWrapper.cpp
    Created: 8 May 2022 9:38:17pm
    Author:  StoneyDSP

  ==============================================================================
*/

#include "PluginWrapper.h"
#include "PluginProcessor.h"

template <typename SampleType>
ProcessWrapper<SampleType>::ProcessWrapper(AudioPluginAudioProcessor& p, APVTS& apvts) : audioProcessor(p)
{
    frequencyPtr = dynamic_cast         <juce::AudioParameterFloat*>        (apvts.getParameter("frequencyID"));
    jassert(frequencyPtr != nullptr);

    resonancePtr = dynamic_cast         <juce::AudioParameterFloat*>        (apvts.getParameter("resonanceID"));
    jassert(resonancePtr != nullptr);

    typePtr = dynamic_cast              <juce::AudioParameterChoice*>       (apvts.getParameter("typeID"));
    jassert(typePtr != nullptr);

    osPtr = dynamic_cast                <juce::AudioParameterChoice*>       (apvts.getParameter("osID"));
    jassert(osPtr != nullptr);

    outputPtr = dynamic_cast            <juce::AudioParameterFloat*>        (apvts.getParameter("outputID"));
    jassert(outputPtr != nullptr);

    mixPtr = dynamic_cast              <juce::AudioParameterFloat*>        (apvts.getParameter("mixID"));
    jassert(mixPtr != nullptr);

    auto osChannels = audioProcessor.getTotalNumInputChannels();

    auto osFilter = juce::dsp::Oversampling<SampleType>::filterHalfBandPolyphaseIIR;

    for (int i = 0; i < 5; ++i)
        overSample[i] = std::make_unique<juce::dsp::Oversampling<SampleType>>
        (osChannels, i, osFilter, true, false);
}

template <typename SampleType>
void ProcessWrapper<SampleType>::setOversampling()
{
    curOS = (int)osPtr->getIndex();
    if (curOS != prevOS)
    {
        overSamplingFactor = 1 << curOS;
        prevOS = curOS;
        svf.reset(static_cast<SampleType>(0.0));
        svf.sampleRate = spec.sampleRate * overSamplingFactor;
        mixer.reset();
        output.reset();
    }
}

template <typename SampleType>
void ProcessWrapper<SampleType>::prepare(double sampleRate, int samplesPerBlock)
{
    overSamplingFactor = 1 << curOS;
    prevOS = curOS;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    for (int i = 0; i < 5; ++i)
        overSample[i]->initProcessing(spec.maximumBlockSize);

    for (int i = 0; i < 5; ++i)
        overSample[i]->numChannels = (size_t)spec.numChannels;

    svf.prepare(spec);
    mixer.prepare(spec);
    output.prepare(spec);

    reset();
    update();
}

template <typename SampleType>
void ProcessWrapper<SampleType>::reset()
{
    svf.reset(static_cast<SampleType>(0.0));
    mixer.reset();
    output.reset();

    for (int i = 0; i < 5; ++i)
        overSample[i]->numChannels = (size_t)spec.numChannels;

    for (int i = 0; i < 5; ++i)
        overSample[i]->reset();
}

template <typename SampleType>
void ProcessWrapper<SampleType>::update()
{
    setOversampling();

    svf.setCutoffFrequency(frequencyPtr->get());
    svf.setResonance(resonancePtr->get());

    switch (typePtr->getIndex())
    {
    case 0:
        svf.setType(StateVariableTPTFilterType::LP2);
        break;
    case 1:
        svf.setType(StateVariableTPTFilterType::LP1);
        break;
    case 2:
        svf.setType(StateVariableTPTFilterType::LP2n);
        break;
    case 3:
        svf.setType(StateVariableTPTFilterType::HP2);
        break;
    case 4:
        svf.setType(StateVariableTPTFilterType::HP1);
        break;
    case 5:
        svf.setType(StateVariableTPTFilterType::HP2n);
        break;
    case 6:
        svf.setType(StateVariableTPTFilterType::BP2);
        break;
    case 7:
        svf.setType(StateVariableTPTFilterType::BP2n);
        break;
    case 8:
        svf.setType(StateVariableTPTFilterType::AP2);
        break;
    case 9:
        svf.setType(StateVariableTPTFilterType::P2);
        break;
    case 10:
        svf.setType(StateVariableTPTFilterType::N2);
        break;
    default:
        svf.setType(StateVariableTPTFilterType::LP2);
    }

    mixer.setWetMixProportion(mixPtr->get() * 0.01f);

    output.setGainLinear(juce::Decibels::decibelsToGain(outputPtr->get()));
}

//==============================================================================
template <typename SampleType>
void ProcessWrapper<SampleType>::process(juce::AudioBuffer<SampleType>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    update();

    juce::dsp::AudioBlock<SampleType> block(buffer);

    mixer.pushDrySamples(block);

    juce::dsp::AudioBlock<SampleType> osBlock = overSample[curOS]->processSamplesUp(block);

    auto context = juce::dsp::ProcessContextReplacing(osBlock);

    svf.process(context);

    output.process(context);

    overSample[curOS]->processSamplesDown(block);

    mixer.mixWetSamples(block);
}

//==============================================================================
template class ProcessWrapper<float>;
template class ProcessWrapper<double>;