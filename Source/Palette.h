/*
  ==============================================================================

    Palette.h
    Created: 3 Feb 2025 8:27:27pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class Palette {
    public:
    Palette(juce::Colour main,
            juce::Colour secondary,
            juce::Colour tertiary,
            juce::Colour border,
            juce::Colour highlight) {
        this->main = main;
        this->secondary = secondary;
        this->tertiary = tertiary;
        this->border = border;
        this->highlight = highlight;
    }
    juce::Colour main;
    juce::Colour secondary;
    juce::Colour tertiary;
    juce::Colour border;
    juce::Colour highlight;
};
