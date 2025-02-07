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
powerAttachment(*p.powerParameter, powerSlider),
zAttachment(*p.zParameter, zSlider),
lengthAttachment(*p.lengthParameter, lengthSlider),
endRateAttachment(*p.endRateParameter, endRateSlider),
envelope(editorInfo, communicators),
pattern(editorInfo, communicators)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 500);
    DBG("editor constructed");
    // power slider
    powerSlider.setSliderStyle(juce::Slider::LinearVertical);
    powerSlider.setRange(0, 5);
    powerAttachment.sendInitialUpdate();
    addAndMakeVisible(powerSlider);
    // z slider
    zSlider.setSliderStyle(juce::Slider::LinearVertical);
    zSlider.setRange(1, 100, 1);
    zAttachment.sendInitialUpdate();
    addAndMakeVisible(zSlider);
    // length slider
    lengthSlider.setSliderStyle(juce::Slider::LinearVertical);
    lengthSlider.setRange(1, 16, 1);
    lengthAttachment.sendInitialUpdate();
    addAndMakeVisible(lengthSlider);
    // end rate combo box
    endRateSlider.setSliderStyle(juce::Slider::LinearVertical);
    endRateSlider.setRange(0, 8, .25);
    endRateAttachment.sendInitialUpdate();
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
    auto patternBounds = bounds.removeFromTop(100);
    pattern.setBounds(patternBounds);
    auto envelopeBounds = bounds.removeFromTop(200);
    envelope.setBounds(envelopeBounds);
    int sliderWidth = bounds.getWidth() / 4;
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    lengthSlider.setBounds(bounds.removeFromLeft(sliderWidth));
    endRateSlider.setBounds(bounds.removeFromLeft(sliderWidth));
    zSlider.setBounds(bounds.removeFromLeft(sliderWidth));
    powerSlider.setBounds(bounds);

}
