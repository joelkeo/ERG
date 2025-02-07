/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EditorInfo.h"
#include "EnvelopeComponent.h"
#include "ArpPatternComponent.h"
#include "ProcessorCommunicators.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&,
                                    ProcessorCommunicators& communicators);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    EditorInfo editorInfo;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;
    juce::Slider powerSlider;
    juce::Slider zSlider;
    juce::Slider lengthSlider;
    juce::Slider endRateSlider;
    // attachments
    juce::SliderParameterAttachment powerAttachment;
    juce::SliderParameterAttachment zAttachment;
    juce::SliderParameterAttachment lengthAttachment;
    juce::SliderParameterAttachment endRateAttachment;
    // keyboard
    EnvelopeComponent envelope;
    ArpPatternComponent pattern;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
