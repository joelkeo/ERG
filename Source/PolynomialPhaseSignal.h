/*
  ==============================================================================

    PolynomialPhaseSignal.h
    Created: 2 Feb 2025 3:38:23pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include "VRPS.h"
class PolynomialPhaseSignal : public VRPS {
    public:
    PolynomialPhaseSignal(juce::AudioParameterFloat& lengthParameter,
         juce::AudioParameterFloat& endRateParameter,
         juce::AudioParameterInt& zParameter,
                          juce::AudioParameterFloat& powerParameter,
                          HostInfo& hostInfo);
    double* getSignal(double* buffer, int startPosition, int length) override;
    protected:
    std::function<double(double)> getPhaseFormula() override;
    std::function<double(double)> getRateFormula() override;
};
