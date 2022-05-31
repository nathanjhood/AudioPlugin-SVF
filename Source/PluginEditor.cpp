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
    addAndMakeVisible(ioButton);
    ioAttach.reset(new APVTS::ButtonAttachment(state, "ioID", ioButton));
    addAndMakeVisible(ioLabel);
    ioLabel.setText("IO", juce::dontSendNotification);
    ioLabel.attachToComponent(&ioButton, true);
    ioLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    ioLabel.setJustificationType(juce::Justification::right);

    addAndMakeVisible(freqSlider);
    freqSlider.TextBoxBelow;
    freqAttach.reset(new APVTS::SliderAttachment(state, "frequencyID", freqSlider));
    addAndMakeVisible(freqLabel);
    freqLabel.setText("Frequency", juce::dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);
    freqLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    freqLabel.setJustificationType(juce::Justification::right);

    addAndMakeVisible(resSlider);
    resSlider.TextBoxBelow;
    resAttach.reset(new APVTS::SliderAttachment(state, "resonanceID", resSlider));
    addAndMakeVisible(freqLabel);
    resLabel.setText("Resonance", juce::dontSendNotification);
    resLabel.attachToComponent(&resSlider, true);
    resLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    resLabel.setJustificationType(juce::Justification::right);

    addAndMakeVisible(typeBox);
    typeAttach.reset(new APVTS::ComboBoxAttachment(state, "typeID", typeBox));

    typeBox.addItem("LP2", 1);
    typeBox.addItem("LP1", 2);
    typeBox.addItem("LP2n", 3);
    typeBox.addItem("HP2", 4);
    typeBox.addItem("HP1", 5);
    typeBox.addItem("HP2n", 6);
    typeBox.addItem("BP2", 7);
    typeBox.addItem("BP2n", 8);
    typeBox.addItem("AP2", 9);
    typeBox.addItem("N2", 10);
    typeBox.addItem("P2", 11);

    typeBox.setSelectedId(1);

    addAndMakeVisible(osBox);
    osAttach.reset(new APVTS::ComboBoxAttachment(state, "osID", osBox));

    osBox.addItem("x1", 1);
    osBox.addItem("x2", 2);
    osBox.addItem("x4", 3);
    osBox.addItem("x8", 4);
    osBox.addItem("x16", 5);

    osBox.setSelectedId(1);
    
    addAndMakeVisible(outputSlider);
    outputAttach.reset(new APVTS::SliderAttachment(state, "outputID", outputSlider));
    addAndMakeVisible(outputLabel);
    outputLabel.setText("Output", juce::dontSendNotification);
    outputLabel.attachToComponent(&outputSlider, true);
    outputLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    outputLabel.setJustificationType(juce::Justification::right);

    addAndMakeVisible(dryWetSlider);
    dryWetAttach.reset(new APVTS::SliderAttachment(state, "mixID", dryWetSlider));
    addAndMakeVisible(dryWetLabel);
    dryWetLabel.setText("Mix", juce::dontSendNotification);
    dryWetLabel.attachToComponent(&dryWetSlider, true);
    dryWetLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    dryWetLabel.setJustificationType(juce::Justification::right);
    
    addAndMakeVisible(bypassButton);
    bypassAttach.reset(new APVTS::ButtonAttachment(state, "bypassID", bypassButton));
    addAndMakeVisible(bypassLabel);
    bypassLabel.setText("Bypass", juce::dontSendNotification);
    bypassLabel.attachToComponent(&bypassButton, true);
    bypassLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    bypassLabel.setJustificationType(juce::Justification::right);

    addAndMakeVisible(displayButton);
    displayAttach.reset(new APVTS::ButtonAttachment(state, "panelID", displayButton));
    addAndMakeVisible(displayLabel);
    displayLabel.setText("GUI", juce::dontSendNotification);
    displayLabel.attachToComponent(&displayButton, true);
    displayLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    displayLabel.setJustificationType(juce::Justification::right);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize (400, 550);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("yourCompany", getLocalBounds(), juce::Justification::topLeft, 1);
    g.drawFittedText(audioProcessor.getName(), getLocalBounds(), juce::Justification::topRight, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto sliderLeft = 100;

    auto separationH = juce::jlimit(2, 10, getHeight() / 20);

    auto width = getWidth();
    auto height = getHeight();

    ioButton.setBounds(sliderLeft, 50, width - sliderLeft - 150, 20);

    freqSlider.setBounds(sliderLeft, 100, width - sliderLeft - 150, 20);
    resSlider.setBounds(sliderLeft, 150, width - sliderLeft - 150, 20);
    typeBox.setBounds(sliderLeft, 200, width - sliderLeft - 150, 20);
    osBox.setBounds(sliderLeft, 250, width - sliderLeft - 150, 20);

    outputSlider.setBounds(sliderLeft, 300, width - sliderLeft - 150, 20);
    dryWetSlider.setBounds(sliderLeft, 350, width - sliderLeft - 150, 20);

    bypassButton.setBounds(sliderLeft, 400, width - sliderLeft - 150, 20);
    displayButton.setBounds(sliderLeft, 450, width - sliderLeft - 150, 20);
}
