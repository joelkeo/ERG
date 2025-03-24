/*
  ==============================================================================

    ExponentialPhaseSignal.h
    Created: 30 Jan 2025 5:10:24pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>
#include "HostInfo.h"
#include "VRPS.h"
#include "MyParameter.h"
class ExponentialPhaseSignal : VRPS {
public:
    ExponentialPhaseSignal(MyParameter& lengthParameter,
                           MyParameter& endRateParameter,
                           MyParameter& zParameter,
                           MyParameter& powerParameter,
                           HostInfo& hostInfo);
    double getLength();
    double* getSignal(double* buffer,
                      int formulaStartPosition,
                      int bufferStartPosition,
                      int bufferLength) override;
private:
    // no reason its called S, just called S
    double getS();
    double getC();
    double getEndRate();
    int getZ();
    // nasty C solving
    void compute();
    double cachedC = 1.;
    double solveC(int targetZ, double endRate, double length);
    double binarySearch(int targetZ, double lowerBounds, double upperBound, double endRate, double length);
    std::vector<double> findBounds(int targetZ, double endRate, double length);
    bool inBounds(int targetZ, double bottom, double top, double endRate, double length);
    double solveZ(double c, double endRate, double length);
};
