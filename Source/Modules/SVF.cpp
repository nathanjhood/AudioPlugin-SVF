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
    filterType = newType;
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::setCutoffFrequency(SampleType newCutoffFrequencyHz)
{
    jassert(juce::isPositiveAndBelow(newCutoffFrequencyHz, static_cast<SampleType> (sampleRate * 0.5)));

    cutoffFrequency = newCutoffFrequencyHz;
    update();
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::setResonance(SampleType newResonance)
{
    jassert(newResonance > static_cast<SampleType> (0));

    resonance = newResonance;
    update();
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

    reset();
    update();
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::reset()
{
    reset(static_cast<SampleType> (0));
}

template <typename SampleType>
void StateVariableTPTFilter<SampleType>::reset(SampleType newValue)
{
    for (auto v : { &s1, &s2 })
        std::fill(v->begin(), v->end(), newValue);
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
    auto& ls1 = s1[(size_t)channel];
    auto& ls2 = s2[(size_t)channel];

    auto yHP = h * (inputValue - ls1 * (g + R2) - ls2);

    auto yBP = yHP * g + ls1;
    ls1 = yHP * g + yBP;

    auto yLP = yBP * g + ls2;
    ls2 = yBP * g + yLP;

    switch (filterType)
    {
    case Type::LP2:         return (yLP);
    case Type::LP1:         return (yLP + yBP);
    case Type::LP2n:        return (yLP * R2);
    case Type::HP2:         return (yHP);
    case Type::HP1:         return (yHP + yBP);
    case Type::HP2n:        return (yHP * R2);
    case Type::BP2:         return (yBP);
    case Type::BP2n:        return (yBP * R2);
    case Type::AP2:         return (inputValue - ((yBP * R2) + (yBP * R2)));
    case Type::P2:          return (yLP - yHP);
    case Type::N2:          return (yLP + yHP);
    default:                return (yLP);
    }

    /*switch (filterType)
    {
    case Type::lowpass:   return yLP;
    case Type::highpass:  return yHP;
    case Type::bandpass:  return yBP;
    case Type::notch:     return (yLP + yHP);
    case Type::peak:      return (yLP - yHP);
    case Type::lp:        return (yLP + yBP);
    case Type::hp:        return (yHP + yBP);
    case Type::bpN:       return (yBP * R2);
    case Type::lowpassN:  return (yLP * R2);
    case Type::highpassN: return (yHP * R2);
    case Type::allpass:   return (inputValue - ((yBP * R2) + (yBP * R2)));
    default:              return yLP;
    }*/
}

//==============================================================================
template <typename SampleType>
void StateVariableTPTFilter<SampleType>::update()
{
    g = static_cast<SampleType> (std::tan(juce::MathConstants<double>::pi * cutoffFrequency / sampleRate));
    R2 = static_cast<SampleType> (1.0 / resonance);
    h = static_cast<SampleType> (1.0 / (1.0 + R2 * g + g * g));
}

//==============================================================================
template class StateVariableTPTFilter<float>;
template class StateVariableTPTFilter<double>;