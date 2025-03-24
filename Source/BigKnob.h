/*
  ==============================================================================

    BigKnob.h
    Created: 8 Feb 2025 6:54:05pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "EditorInfo.h"
#include "MyParameter.h"
class BigKnob : public juce::Slider {
    public:
    BigKnob(EditorInfo& editorInfo,
            MyParameter& parameter);
    void paint(juce::Graphics& g) override;
    private:
    EditorInfo& editorInfo;
    juce::RangedAudioParameter& parameter;
    juce::SliderParameterAttachment attachment;
};
