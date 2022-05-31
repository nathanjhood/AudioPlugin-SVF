/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p, APVTS& apvts)
    : AudioProcessorEditor (&p), audioProcessor(p), state(apvts)
{
    addAndMakeVisible(freqSlider);
    freqAttach.reset(new APVTS::SliderAttachment(state, "frequencyID", freqSlider));

    addAndMakeVisible(resSlider);
    resAttach.reset(new APVTS::SliderAttachment(state, "resonanceID", resSlider));

    addAndMakeVisible(typeBox);
    typeAttach.reset(new APVTS::ComboBoxAttachment(state, "typeID", typeBox));
    
    addAndMakeVisible(outputSlider);
    outputAttach.reset(new APVTS::SliderAttachment(state, "outputID", outputSlider));

    addAndMakeVisible(dryWetSlider);
    dryWetAttach.reset(new APVTS::SliderAttachment(state, "mixID", dryWetSlider));

    addAndMakeVisible(bypassButton);
    bypassAttach.reset(new APVTS::ButtonAttachment(state, "bypassID", bypassButton));

    addAndMakeVisible(displayButton);
    displayAttach.reset(new APVTS::ButtonAttachment(state, "panelID", displayButton));

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize (400, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("yourCompany", getLocalBounds(), juce::Justification::topLeft, 1);
    g.drawFittedText(audioProcessor.getName(), getLocalBounds(), juce::Justification::topRight, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto sliderLeft = 30;
    freqSlider.setBounds(sliderLeft, 300, getWidth() - sliderLeft - 10, 20);
    resSlider.setBounds(sliderLeft, 350, getWidth() - sliderLeft - 10, 20);
    typeBox.setBounds(sliderLeft, 400, getWidth() - sliderLeft - 10, 20);

    outputSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    dryWetSlider.setBounds(sliderLeft, 100, getWidth() - sliderLeft - 10, 20);

    bypassButton.setBounds(sliderLeft, 150, getWidth() - sliderLeft - 10, 20);
    displayButton.setBounds(sliderLeft, 200, getWidth() - sliderLeft - 10, 20);
}
