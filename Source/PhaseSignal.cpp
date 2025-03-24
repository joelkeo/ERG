/*
  ==============================================================================

    PhaseSignal.cpp
    Created: 31 Jan 2025 12:09:36pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "PhaseSignal.h"
PhaseSignal::PhaseSignal(MyParameter& lengthParameter,
                         MyParameter& endRateParameter,
                         MyParameter& zParameter,
                         MyParameter& powerParameter,
            HostInfo& hostInfo) :
vrps(lengthParameter, endRateParameter, zParameter, powerParameter, hostInfo),
hostInfo(hostInfo),
attachment(&vrps) {}

double* PhaseSignal::process(std::vector<PhaseSignalEvent> events, int length) {
    // simple phase signal event approach
    clearBuffer(length);
    bool justStarted = false;
    int lastStart = 0;
    for (PhaseSignalEvent event : events) {
        if (event.turnOn) {
            lastStart = event.position;
            isOn = true;
        }
        else {
            int endPosition = event.position;
            requestSignal(phaseBuffer,
                           startPosition,
                           lastStart,
                           endPosition - lastStart);
            startPosition += (endPosition - lastStart);
            vrps.stop();
            startPosition = 0;
            isOn = false;
            DBG("END EVENT");
        }
    }
    if (! isOn) {return phaseBuffer;}
    requestSignal(phaseBuffer,
                   startPosition,
                   lastStart,
                   length - lastStart);
    startPosition += (length - lastStart);
    return phaseBuffer;
    /*
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
     */
}

void PhaseSignal::clearBuffer(int length) {
    for (int i = 0 ; i < length; i++) {
        phaseBuffer[i] = -1.;
    }
}

// HANDLES TRUNCATING CALL TO VRPS
void PhaseSignal::requestSignal(double* buffer,
                   int formulaStart,
                   int bufferStart,
                   int bufferLength) {
    // no truncating case
    if (! (bufferLength + formulaStart > vrps.getLength())) {
        vrps.getSignal(buffer, formulaStart, bufferStart, bufferLength);
    }
    // yes truncating case
    else {
        int newLength = vrps.getLength() - formulaStart;
        vrps.getSignal(buffer, formulaStart, bufferStart, newLength);
        isOn = false;
        startPosition = 0;
        vrps.stop();
    }
}
