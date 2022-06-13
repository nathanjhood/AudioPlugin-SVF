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
ProcessWrapper<SampleType>::ProcessWrapper(AudioPluginAudioProcessor& p, APVTS& apvts) 
    :
    audioProcessor(p),
    state(apvts)
{
    spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    frequencyPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("frequencyID"));
    resonancePtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("resonanceID"));
    typePtr = dynamic_cast <juce::AudioParameterChoice*> (state.getParameter("typeID"));
    osPtr = dynamic_cast <juce::AudioParameterChoice*> (state.getParameter("osID"));
    outputPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("outputID"));
    mixPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("mixID"));
    
    jassert(frequencyPtr != nullptr);
    jassert(resonancePtr != nullptr);
    jassert(typePtr != nullptr);
    jassert(osPtr != nullptr);
    jassert(outputPtr != nullptr);
    jassert(mixPtr != nullptr);

    auto osChannels = audioProcessor.getTotalNumInputChannels();

    auto osFilter = juce::dsp::Oversampling<SampleType>::filterHalfBandPolyphaseIIR;

    for (int i = 0; i < 5; ++i)
        overSample[i] = std::make_unique<juce::dsp::Oversampling<SampleType>>
        (osChannels, i, osFilter, true, false);
}

template <typename SampleType>
void ProcessWrapper<SampleType>::prepare()
{
    overSamplingFactor = 1 << curOS;
    prevOS = curOS;

    spec.sampleRate = audioProcessor.getSampleRate() * overSamplingFactor;
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    for (int i = 0; i < 5; ++i)
        overSample[i]->initProcessing(spec.maximumBlockSize);

    for (int i = 0; i < 5; ++i)
        overSample[i]->numChannels = (size_t)spec.numChannels;

    mixer.prepare(spec);
    svf.prepare(spec);
    output.prepare(spec);

    reset();
    update();
}

template <typename SampleType>
void ProcessWrapper<SampleType>::reset()
{
    mixer.reset();
    mixer.setWetLatency(getLatencySamples());
    svf.reset(static_cast<SampleType>(0.0));
    output.reset();

    for (int i = 0; i < 5; ++i)
        overSample[i]->numChannels = (size_t)spec.numChannels;

    for (int i = 0; i < 5; ++i)
        overSample[i]->reset();
}

//==============================================================================
template <typename SampleType>
void ProcessWrapper<SampleType>::process(juce::AudioBuffer<SampleType>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    update();

    juce::dsp::AudioBlock<SampleType> block(buffer);
    juce::dsp::AudioBlock<SampleType> osBlock(buffer);

    mixer.pushDrySamples(block);

    osBlock = overSample[curOS]->processSamplesUp(block);

    auto context = juce::dsp::ProcessContextReplacing(osBlock);

    svf.process(context);

    output.process(context);

    overSample[curOS]->processSamplesDown(block);

    mixer.mixWetSamples(block);
}

template <typename SampleType>
SampleType ProcessWrapper<SampleType>::getLatencySamples() const noexcept
{
    // latency of oversampling
    return overSample[curOS]->getLatencyInSamples();
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
        mixer.reset();
        mixer.setWetLatency(getLatencySamples());
        output.reset();
    }
}

template <typename SampleType>
void ProcessWrapper<SampleType>::update()
{
    spec.sampleRate = audioProcessor.getSampleRate() * overSamplingFactor;
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    setOversampling();

    mixer.setWetMixProportion(mixPtr->get() * 0.01f);

    svf.setCutoffFrequency(frequencyPtr->get());
    svf.setResonance(resonancePtr->get());
    svf.setType(static_cast<StateVariableTPTFilterType>(typePtr->getIndex()));

    output.setGainLinear(juce::Decibels::decibelsToGain(outputPtr->get()));
}

//==============================================================================
template class ProcessWrapper<float>;
template class ProcessWrapper<double>;