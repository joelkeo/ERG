/*
  ==============================================================================

    PhaseSignal.h
    Created: 31 Jan 2025 12:09:36pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include "PhaseSignalEvent.h"
#include <JuceHeader.h>
#include "ExponentialPhaseSignal.h"
#include "PolynomialPhaseSignal.h"
#include "HostInfo.h"
#include "VRPSEnvelopeAttachment.h"
#include "MyParameter.h"
class PhaseSignal {
    public:
    PhaseSignal(MyParameter& lengthParameter,
                MyParameter& endRateParameter,
                MyParameter& zParameter,
                MyParameter& powerParameter,
                HostInfo& hostInfo);
    double* process(std::vector<PhaseSignalEvent>, int length);
    double phaseBuffer[10000];
    PolynomialPhaseSignal vrps;
    VRPSEnvelopeAttachment attachment;
    int startPosition = 0;
    bool isOn = false;
    void rhythmicSignalProcess(int start, int end);
    void clearBuffer(int length);
    private:
    HostInfo& hostInfo;
    void requestSignal(double* buffer,
                       int formulaStart,
                       int bufferStart,
                       int bufferLength);
};
