/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p,ProcessorCommunicators& communicators)
    : AudioProcessorEditor (&p), audioProcessor (p),
powerSlider(editorInfo, *p.powerParameter),
zSlider(editorInfo, *p.zParameter),
lengthSlider(editorInfo, *p.lengthParameter),
endRateSlider(editorInfo, *p.endRateParameter),
envelope(editorInfo, communicators),
pattern(editorInfo, communicators)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 500);
    DBG("editor constructed");
    // power slider
    powerSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    powerSlider.setRange(0, 5);
    addAndMakeVisible(powerSlider);
    // z slider
    zSlider.setSliderStyle(juce::Slider::LinearVertical);
    zSlider.setRange(1, 100, 1);
    addAndMakeVisible(zSlider);
    // length slider
    lengthSlider.setSliderStyle(juce::Slider::LinearVertical);
    lengthSlider.setRange(1, 16, 1);
    addAndMakeVisible(lengthSlider);
    // end rate combo box
    endRateSlider.setSliderStyle(juce::Slider::LinearVertical);
    endRateSlider.setRange(0, 8, .25);
    addAndMakeVisible(endRateSlider);
    // keybaord
    addAndMakeVisible(&envelope);
    addAndMakeVisible(&pattern);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    DBG("editor destroyed");
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (editorInfo.main());
}

void NewProjectAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto patternBounds = bounds.removeFromTop(200);
    pattern.setBounds(patternBounds);
    auto envelopeBounds = bounds.removeFromTop(150);
    envelope.setBounds(envelopeBounds);
    int sliderWidth = bounds.getWidth() / 4;
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    lengthSlider.setBounds(bounds.removeFromLeft(sliderWidth));
    endRateSlider.setBounds(bounds.removeFromLeft(sliderWidth));
    zSlider.setBounds(bounds.removeFromLeft(sliderWidth));
    powerSlider.setBounds(bounds);
}
