/*
  ==============================================================================

    ArpPatternComponent.h
    Created: 6 Feb 2025 11:57:37am
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include "ProcessorCommunicators.h"
#include "EditorInfo.h"
class ArpPatternComponent : public juce::Component,
public juce::Timer{
    public:
    ArpPatternComponent(EditorInfo& editorInfo,
                        ProcessorCommunicators& communicators);
    ~ArpPatternComponent();
    void paint(juce::Graphics& g) override;
    void timerCallback() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseDoubleClick(const juce::MouseEvent& e) override;
    private:
    EditorInfo& editorInfo;
    ArpPattern& pattern;
    NoteSelectionInfo selection;
    double getNormalizedX(int x);
    double getNormalizedY(int y);
    juce::String noteNumToString(int num);
};
