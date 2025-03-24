/*
  ==============================================================================

    VRPS.cpp
    Created: 2 Feb 2025 3:42:22pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "VRPS.h"
VRPS::VRPS(MyParameter& lengthParameter,
           MyParameter& endRateParameter,
           MyParameter& zParameter,
           MyParameter& powerParameter,
           HostInfo& hostInfo) : lengthParameter(lengthParameter),
endRateParameter(endRateParameter),
zParameter(zParameter),
powerParameter(powerParameter),
hostInfo(hostInfo) {
    lengthParameter.addListener(this);
    endRateParameter.addListener(this);
    zParameter.addListener(this);
    powerParameter.addListener(this);
}

int VRPS::getLength() {
    double bpm = hostInfo.bpm;
    double sampleRate = hostInfo.sampleRate;
    return (lengthParameter.getAdjustedValue() / (bpm / 60.)) * sampleRate;
}
int VRPS::getLengthBeats() {
    return lengthParameter.getAdjustedValue();
}
double VRPS::getEndRateBeats() {
    return endRateParameter.getAdjustedValue();
}
double VRPS::getEndRate() {
    double bpm = hostInfo.bpm;
    double sampleRate = hostInfo.sampleRate;
    double result = (endRateParameter.getAdjustedValue() * (bpm / 60.)) / sampleRate;
    return result;
}
int VRPS::getZ() {
    return zParameter.getAdjustedValue();
}
void VRPS::addListener(EnvelopeListener* listener) {
    listeners.push_back(listener);
    formulaChanged();
}
void VRPS::removeListener(EnvelopeListener* listener) {
    auto it = std::find(listeners.begin(), listeners.end(), listener);
    // Remove it if found
    if (it != listeners.end()) {listeners.erase(it);}
}
void VRPS::formulaChanged() {
    DBG("FORMULA CHANGED###");
    DBG("LENGTH: " << lengthParameter.getAdjustedValue());
    DBG("ENDRATE: " << endRateParameter.getAdjustedValue());
    DBG("z " << zParameter.getAdjustedValue());
    DBG("Power " << powerParameter.getAdjustedValue());
    for (auto listener : listeners) {
        listener->updateFormula(getPhaseFormula(), getRateFormula(), getLengthBeats());
        listener->clearPosition();
    }
}
void VRPS::positionUpdated(double position, int zIndex, double moddedValue) {
    for (auto listener : listeners) {
        listener->updatePosition(position, zIndex, moddedValue);
    }
}
void VRPS::parameterValueChanged(int, float) {
    formulaChanged();
}
void VRPS::parameterGestureChanged(int, bool) {}

void VRPS::stop() {
    for (auto listener : listeners) {
        listener->clearPosition();
    }
}
