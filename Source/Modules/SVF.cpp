/*
  ==============================================================================
    SVF.cpp
    Created: 25 Mar 2022 11:49:36pm
    Author:  StoneyDSP
  ==============================================================================
*/

#include "SVF.h"

//==============================================================================
template <typename SampleType>
StateVariableTPTFilter<SampleType>::StateVariableTPTFilter()
{
    update();
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::setType(Type newType)
{
    if (filterType != newType)
    {
        filterType = newType;
        reset(static_cast<SampleType>(0.0));
        update();
    }
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::setCutoffFrequency(SampleType newFreq)
{
    jassert(juce::isPositiveAndBelow(newFreq, static_cast<SampleType> (sampleRate * 0.5)));
    jassert(static_cast<SampleType>(20.0) <= newFreq && newFreq <= static_cast<SampleType>(20000.0));

    cutoffFrequency = newFreq;
    frq.setTargetValue(cutoffFrequency);
    update();
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::setResonance(SampleType newResonance)
{
    jassert(newResonance > static_cast<SampleType> (0));
    jassert(static_cast<SampleType>(0.0) <= newResonance && newResonance <= static_cast<SampleType>(1.0));

    resonance = newResonance;
    res.setTargetValue(resonance);
    update();
}

//==============================================================================
template <typename SampleType>
void StateVariableTPTFilter<SampleType>::setRampDurationSeconds(double newDurationSeconds) noexcept
{
    if (rampDurationSeconds != newDurationSeconds)
    {
        rampDurationSeconds = newDurationSeconds;
        reset(static_cast<SampleType>(0.0));
    }
}

template <typename SampleType>
double StateVariableTPTFilter<SampleType>::getRampDurationSeconds() const noexcept
{
    return rampDurationSeconds;
}

template <typename SampleType>
bool StateVariableTPTFilter<SampleType>::isSmoothing() const noexcept
{
    bool compSmoothing = frq.isSmoothing() || res.isSmoothing();

    return compSmoothing;
}

//==============================================================================
template <typename SampleType>
void StateVariableTPTFilter<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    jassert(spec.sampleRate > 0);
    jassert(spec.numChannels > 0);

    sampleRate = spec.sampleRate;

    s1.resize(spec.numChannels);
    s2.resize(spec.numChannels);

    minFreq = static_cast <SampleType>(sampleRate) / static_cast <SampleType>(24576.0);
    maxFreq = static_cast <SampleType>(sampleRate) / static_cast <SampleType>(2.125);

    jassert(static_cast <SampleType>(20.0) >= minFreq && minFreq <= static_cast <SampleType>(20000.0));
    jassert(static_cast <SampleType>(20.0) <= maxFreq && maxFreq >= static_cast <SampleType>(20000.0));

    reset(static_cast<SampleType>(0.0));
    update();
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::reset(SampleType newValue)
{
    for (auto v : { &s1, &s2 })
        std::fill(v->begin(), v->end(), newValue);

    frq.reset(sampleRate, rampDurationSeconds);
    res.reset(sampleRate, rampDurationSeconds);
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::snapToZero() noexcept
{
    for (auto v : { &s1, &s2 })
        for (auto& element : *v)
            juce::dsp::util::snapToZero(element);
}

//==============================================================================
template <typename SampleType>
SampleType StateVariableTPTFilter<SampleType>::processSample(int channel, SampleType inputValue)
{
    jassert(juce::isPositiveAndBelow(channel, s1.size()));
    jassert(juce::isPositiveAndBelow(channel, s2.size()));

    auto& ls1 = s1[(size_t)channel];
    auto& ls2 = s2[(size_t)channel];

    auto yHP = h * (inputValue - ls1 * (g + R2) - ls2);

    auto yBP = yHP * g + ls1;
    ls1 = yHP * g + yBP;

    auto yLP = yBP * g + ls2;
    ls2 = yBP * g + yLP;

    if (filterType == Type::LP2)
        return (yLP);
    else if (filterType == Type::LP1)
        return (yLP + yBP);
    else if (filterType == Type::LP2n)
        return (yLP * R2);
    else if (filterType == Type::HP2)
        return (yHP);
    else if (filterType == Type::HP1)
        return (yHP + yBP);
    else if (filterType == Type::HP2n)
        return (yHP * R2);
    else if (filterType == Type::BP2)
        return (yBP);
    else if (filterType == Type::BP2n)
        return (yBP * R2);
    else if (filterType == Type::AP2)
        return (inputValue - ((yBP * R2) + (yBP * R2)));
    else if (filterType == Type::P2)
        return (yLP + yHP);
    else if (filterType == Type::N2)
        return (yLP - yHP);
    else
        return (yLP);
}

//==============================================================================
template <typename SampleType>
void StateVariableTPTFilter<SampleType>::update()
{
    g = static_cast<SampleType> (std::tan(juce::MathConstants<double>::pi * frq.getNextValue() / sampleRate));
    R2 = static_cast<SampleType> (1.0 - (res.getNextValue() * 0.9875)) + (1.0 - (res.getNextValue() * (static_cast<SampleType>(0.9875))));
    h = static_cast<SampleType> (1.0 / (1.0 + R2 * g + g * g));
}

//==============================================================================
template class StateVariableTPTFilter<float>;
template class StateVariableTPTFilter<double>;