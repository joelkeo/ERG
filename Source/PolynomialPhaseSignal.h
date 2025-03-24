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
    PolynomialPhaseSignal(MyParameter& lengthParameter,
                          MyParameter& endRateParameter,
                          MyParameter& zParameter,
                          MyParameter& powerParameter,
                          HostInfo& hostInfo);
    double* getSignal(double* buffer,
                      int formulaStartPosition,
                      int bufferStartPosition,
                      int bufferLength) override;
    protected:
    std::function<double(double)> getPhaseFormula() override;
    std::function<double(double)> getRateFormula() override;
};
