/*
  ==============================================================================

    VRPS.h
    Created: 2 Feb 2025 3:38:03pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include "HostInfo.h"
#include "EnvelopeListener.h"
#include <JuceHeader.h>
#include "MyParameter.h"

class VRPS : juce::AudioProcessorParameter::Listener {
    public:
    virtual ~VRPS() {}
    VRPS(MyParameter& lengthParameter,
         MyParameter& endRateParameter,
         MyParameter& zParameter,
         MyParameter& powerParameter,
         HostInfo& hostInfo);
    virtual double* getSignal(double* buffer,
                              int formulaStartPosition,
                              int bufferStartPosition,
                              int bufferLength) = 0;
    int getLength();
    int getLengthBeats();
    void addListener(EnvelopeListener* listener);
    void removeListener(EnvelopeListener* listener);
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override;
    void stop();
    protected:
    void formulaChanged();
    void positionUpdated(double position, int zIndex, double moddedValue);
    virtual std::function<double(double)> getPhaseFormula() = 0;
    virtual std::function<double(double)> getRateFormula() = 0;
    double getEndRate();
    double getEndRateBeats();
    int getZ();
    MyParameter& lengthParameter;
    MyParameter& endRateParameter;
    MyParameter& zParameter;
    MyParameter& powerParameter;
    HostInfo& hostInfo;
    private:
    std::vector<EnvelopeListener*> listeners;
};
