/*
  ==============================================================================

    EditorInfo.cpp
    Created: 3 Feb 2025 8:27:48pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "EditorInfo.h"
EditorInfo::EditorInfo() {
    palette = &pinky;
}
juce::Colour EditorInfo::main() {
    return palette->main;
}
juce::Colour EditorInfo::secondary() {
    return palette->secondary;
}
juce::Colour EditorInfo::tertiary() {
    return palette->tertiary;
}
juce::Colour EditorInfo::border() {
    return palette->border;
}
juce::Colour EditorInfo::highlight() {
    return palette->highlight;
}
