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
    Palette pinky = Palette(juce::Colour(255, 156, 231),
                            juce::Colour(182,35,160),
                            juce::Colour(255, 219, 247),
                            juce::Colour(107,76,101),
                            juce::Colour(194,202,205));
};
