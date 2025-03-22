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
    Palette pinky = Palette(juce::Colour(253,197,244),
                            juce::Colour(187,50,165),
                            juce::Colour(228,169,175),
                            juce::Colour(107,76,101),
                            juce::Colour(255, 156, 231));
};
