/*
  ==============================================================================

    ArpNote.h
    Created: 6 Feb 2025 12:44:51pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class ArpNote {
    public:
    ArpNote(int noteStart, int noteEnd, int noteNum, float velocity);
    void stabilize();
    int noteNum;
    int currentStart;
    int currentEnd;
    int stableStart;
    int stableEnd;
    float velocity;
    // JSON SHIT
    ArpNote(juce::var json);
    juce::var getJSON();
};
