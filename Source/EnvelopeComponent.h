/*
  ==============================================================================

    EnvelopeComponent.h
    Created: 4 Feb 2025 3:02:48pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "EditorInfo.h"
#include "VRPSEnvelopeAttachment.h"
#include "ProcessorCommunicators.h"
class EnvelopeComponent : public juce::Component,
public EnvelopeListener,
public juce::Timer {
    public:
    EnvelopeComponent(EditorInfo& editorInfo,
                      ProcessorCommunicators& communicators);
    ~EnvelopeComponent();
    void paint(juce::Graphics& g) override;
    // listeners
    void updatePosition(double position, int zIndex, double moddedValue) override;
    void clearPosition() override;
    void updateFormula(std::function<double(double)> phaseFormula,
                  std::function<double(double)> rateFormula,
                  int length) override;
    void timerCallback() override;

    private:
    VRPSEnvelopeAttachment& attachment;
    juce::Path phasePath;
    std::vector<double> lineXPositions;
    std::vector<double> lineYScalings;
    juce::Path ratePath;
    EditorInfo& editorInfo;
    bool showingPosition = false;
    double position = 0.;
    int zIndex = 0.;
    double moddedValue = 0.;
    int length = 1;
};
