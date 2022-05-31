/*
  ==============================================================================
    SVF.h
    Created: 25 Mar 2022 11:49:36pm
    Author:  StoneyDSP
  ==============================================================================
*/

#pragma once

#ifndef SVF_H_INCLUDED
#define SVF_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

enum class StateVariableTPTFilterType
{
    LP2,
    LP1,
    LP2n,
    HP2,
    HP1,
    HP2n,
    BP2,
    BP2n,
    AP2,
    N2,
    P2
};

//==============================================================================
/** An IIR filter that can perform low, band and high-pass filtering on an audio
    signal, with 12 dB of attenuation per octave, using a TPT structure, designed
    for fast modulation (see Vadim Zavalishin's documentation about TPT
    structures for more information). Its behaviour is based on the analog
    state variable filter circuit.
    Note: The bandpass here is not the one in the RBJ CookBook as its gain can be
    higher than 0 dB. For the classic 0 dB bandpass, we need to multiply the
    result by R2.
    Note 2: Using this class prevents some loud audio artefacts commonly encountered when
    changing the cutoff frequency using other filter simulation structures and IIR
    filter classes. However, this class may still require additional smoothing for
    cutoff frequency changes.
    see IIRFilter, SmoothedValue
    @tags{DSP}
*/
template <typename SampleType>
class StateVariableTPTFilter
{
public:
    //==========================================================================
    using Type = StateVariableTPTFilterType;

    //==========================================================================
    /** Constructor. */
    StateVariableTPTFilter();

    //==========================================================================
    /** Sets the filter type. */
    void setType(Type newType);

    /** Sets the cutoff frequency of the filter.
        @param newFrequencyHz the new cutoff frequency in Hz.
    */
    void setCutoffFrequency(SampleType newFrequencyHz);

    /** Sets the resonance of the filter.
        Note: The bandwidth of the resonance increases with the value of the
        parameter. To have a standard 12 dB / octave filter, the value must be set
        at 1 / sqrt(2).
    */
    void setResonance(SampleType newResonance);

    //==========================================================================
    /** Returns the type of the filter. */
    Type getType() const noexcept { return filterType; }

    /** Returns the cutoff frequency of the filter. */
    SampleType getCutoffFrequency() const noexcept { return cutoffFrequency; }

    /** Returns the resonance of the filter. */
    SampleType getResonance() const noexcept { return resonance; }

    //==============================================================================
    /** Sets the length of the ramp used for smoothing parameter changes. */
    void setRampDurationSeconds(double newDurationSeconds) noexcept;

    /** Returns the ramp duration in seconds. */
    double getRampDurationSeconds() const noexcept;

    /** Returns true if the current value is currently being interpolated. */
    bool isSmoothing() const noexcept;

    //==========================================================================
    /** Initialises the filter. */
    void prepare(const juce::dsp::ProcessSpec& spec);

    /** Resets the internal state variables of the filter to a given value. */
    void reset(SampleType newValue);

    /** Ensure that the state variables are rounded to zero if the state
        variables are denormals. This is only needed if you are doing
        sample by sample processing.
    */
    void snapToZero() noexcept;

    //==========================================================================
    /** Processes the input and output samples supplied in the processing context. */
    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples = outputBlock.getNumSamples();
        const auto len = inputBlock.getNumSamples();

        jassert(inputBlock.getNumChannels() <= s1.size());
        jassert(inputBlock.getNumChannels() == numChannels);
        jassert(inputBlock.getNumSamples() == numSamples);

        if (context.isBypassed)
        {
            frq.skip(static_cast<int> (len));
            res.skip(static_cast<int> (len));

            outputBlock.copyFrom(inputBlock);
            return;
        }

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples = inputBlock.getChannelPointer(channel);
            auto* outputSamples = outputBlock.getChannelPointer(channel);

            for (size_t i = 0; i < numSamples; ++i)
                outputSamples[i] = processSample((int)channel, inputSamples[i]);
        }

#if JUCE_DSP_ENABLE_SNAP_TO_ZERO
        snapToZero();
#endif
    }

    //==========================================================================
    /** Processes one sample at a time on a given channel. */
    SampleType processSample(int channel, SampleType inputValue);

    double sampleRate = 44100.0, rampDurationSeconds = 0.00005;

private:
    //==========================================================================
    void update();

    //==========================================================================
    /** Parameter Smoothers. */
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> frq;
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Linear> res;

    SampleType minFreq = 20.0;
    SampleType maxFreq = 20000.0;

    //==========================================================================
    SampleType g, h, R2;
    std::vector<SampleType> s1{ 2 }, s2{ 2 };

    //==========================================================================
    Type filterType = Type::LP2;
    SampleType cutoffFrequency = 1000.0; 
    SampleType resonance = 0.70710678118654752440084436210485;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateVariableTPTFilter)
};

#endif //SVF_H_INCLUDED
