/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginSVFAudioProcessor::AudioPluginSVFAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
    apvts(*this, &undoManager, "Parameters", createParameterLayout()),
    spec(),
    parameters(*this, getAPVTS()),
    processorFloat(*this, getAPVTS(), getSpec()),
    processorDouble(*this, getAPVTS(), getSpec())
{
}

AudioPluginSVFAudioProcessor::~AudioPluginSVFAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorParameter* AudioPluginSVFAudioProcessor::getBypassParameter() const
{
    return bypassState;
}

bool AudioPluginSVFAudioProcessor::isBypassed() const noexcept
{
    return bypassState->get() == true;
}

void AudioPluginSVFAudioProcessor::setBypassParameter(juce::AudioParameterBool* newBypass) noexcept
{
    if (bypassState != newBypass)
    {
        bypassState = newBypass;
        releaseResources();
        reset();
    }
}

bool AudioPluginSVFAudioProcessor::supportsDoublePrecisionProcessing() const
{
    return false;
}

juce::AudioProcessor::ProcessingPrecision AudioPluginSVFAudioProcessor::getProcessingPrecision() const noexcept
{
    return processingPrecision;
}

bool AudioPluginSVFAudioProcessor::isUsingDoublePrecision() const noexcept
{
    return processingPrecision == doublePrecision;
}

void AudioPluginSVFAudioProcessor::setProcessingPrecision(ProcessingPrecision newPrecision) noexcept
{
    // If you hit this assertion then you're trying to use double precision
    // processing on a processor which does not support it!
    jassert(newPrecision != doublePrecision || supportsDoublePrecisionProcessing());

    if (processingPrecision != newPrecision)
    {
        processingPrecision = newPrecision;
        releaseResources();
        reset();
    }
}

//==============================================================================
const juce::String AudioPluginSVFAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginSVFAudioProcessor::acceptsMidi() const
{
    return false;
}

bool AudioPluginSVFAudioProcessor::producesMidi() const
{
    return false;
}

bool AudioPluginSVFAudioProcessor::isMidiEffect() const
{
    return false;
}

double AudioPluginSVFAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginSVFAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginSVFAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginSVFAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginSVFAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginSVFAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}

//==============================================================================
void AudioPluginSVFAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    getProcessingPrecision();

    processorFloat.prepare(getSpec());
    processorDouble.prepare(getSpec());
}

void AudioPluginSVFAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

    processorFloat.reset();
    processorDouble.reset();
}

void AudioPluginSVFAudioProcessor::numChannelsChanged()
{
    processorFloat.reset();
    processorDouble.reset();
    processorFloat.prepare(getSpec());
    processorDouble.prepare(getSpec());
}

void AudioPluginSVFAudioProcessor::numBusesChanged()
{
    processorFloat.reset();
    processorDouble.reset();
    processorFloat.prepare(getSpec());
    processorDouble.prepare(getSpec());
}

void AudioPluginSVFAudioProcessor::processorLayoutsChanged()
{
    processorFloat.reset();
    processorDouble.reset();
    processorFloat.prepare(getSpec());
    processorDouble.prepare(getSpec());
}

bool AudioPluginSVFAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

//==============================================================================
void AudioPluginSVFAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (bypassState->get() == false)

    {
        juce::ScopedNoDenormals noDenormals;

        processorFloat.process(buffer, midiMessages);
    }

    else
    {
        processBlockBypassed(buffer, midiMessages);
    }
}

void AudioPluginSVFAudioProcessor::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    if (bypassState->get() == false)

    {
        juce::ScopedNoDenormals noDenormals;

        processorDouble.process(buffer, midiMessages);
    }

    else
    {
        processBlockBypassed(buffer, midiMessages);
    }
}

void AudioPluginSVFAudioProcessor::processBlockBypassed(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(buffer);
    juce::ignoreUnused(midiMessages);
}

void AudioPluginSVFAudioProcessor::processBlockBypassed(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(buffer);
    juce::ignoreUnused(midiMessages);
}

//==============================================================================
bool AudioPluginSVFAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginSVFAudioProcessor::createEditor()
{
    return new AudioPluginSVFAudioProcessorEditor(*this, getAPVTS(), undoManager);
}

//==============================================================================
void AudioPluginSVFAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginSVFAudioProcessor::getCurrentProgramStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginSVFAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

void AudioPluginSVFAudioProcessor::setCurrentProgramStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginSVFAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout params;

    Parameters::setParameterLayout(params);

    return params;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginSVFAudioProcessor();
}
