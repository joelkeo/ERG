/*
  ==============================================================================

    PhaseSignal.cpp
    Created: 31 Jan 2025 12:09:36pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "PhaseSignal.h"
PhaseSignal::PhaseSignal(juce::AudioParameterFloat& lengthParameter,
            juce::AudioParameterFloat& endRateParameter,
            juce::AudioParameterInt& zParameter,
                         juce::AudioParameterFloat& powerParameter,
            HostInfo& hostInfo) :
vrps(lengthParameter, endRateParameter, zParameter, powerParameter, hostInfo),
hostInfo(hostInfo),
attachment(&vrps) {}

double* PhaseSignal::process(std::vector<PhaseSignalEvent> events, int length) {
    // simple phase signal event approach
    clearBuffer(length);
    bool justStarted = false;
    for (PhaseSignalEvent event : events) {
        if (event.turnOn) {
            startPosition = 0;
            isOn = true;
            justStarted = true;
            DBG("START EVENT");
        }
        else {
            isOn = false;
            DBG("END EVENT");
        }
    }
    if (! isOn) {return phaseBuffer;}
    int expLength = vrps.getLength();
    int endPosition = startPosition + length;
    // all rhythmic case
    if (startPosition > expLength) {
        vrps.stop();
    }
    // partially rhytmic case
    else if (endPosition > expLength) {
        vrps.stop();
    }
    // normal case
    else {
        vrps.getSignal(phaseBuffer, startPosition, endPosition - startPosition);
    }
    startPosition += length;
    return phaseBuffer;
}

void PhaseSignal::clearBuffer(int length) {
    for (int i = 0 ; i < length; i++) {
        phaseBuffer[i] = -1.;
    }
}
