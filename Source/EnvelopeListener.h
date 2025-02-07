/*
  ==============================================================================

    EnvelopeListener.h
    Created: 4 Feb 2025 3:02:59pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class EnvelopeListener {
    public:
    virtual ~EnvelopeListener() {};
    virtual void updatePosition(double position, int zIndex, double moddedValue) = 0;
    virtual void clearPosition() = 0;
    virtual void updateFormula(std::function<double(double)> phaseFormula,
                  std::function<double(double)> rateFormula,
                  int length) = 0;
};
