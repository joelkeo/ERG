/*
  ==============================================================================

    EditorInfo.h
    Created: 3 Feb 2025 8:27:48pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include "Palette.h"
class EditorInfo {
    public:
    EditorInfo();
    juce::Colour main();
    juce::Colour secondary();
    juce::Colour tertiary();
    juce::Colour border();
    juce::Colour highlight();
    private:
    Palette* palette;
    Palette pinky = Palette(juce::Colour(242,240,241),
                            juce::Colour(178,25,156),
                            juce::Colour(216,125,147),
                            juce::Colour(107,76,101),
                            juce::Colour(255, 156, 231));
};
