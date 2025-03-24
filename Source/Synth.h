/*
  ==============================================================================

    Synth.h
    Created: 31 Jan 2025 11:51:58am
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MidiScheduler.h"
#include "ArpPattern.h"
#include "PhaseSignal.h"
#include "PhaseSignalEvent.h"
#include "HostInfo.h"
#include "ProcessorCommunicators.h"
#include "MyParameter.h"
class Synth {
    public:
    Synth(MyParameter& lengthParameter,
          MyParameter& endRateParameter,
          MyParameter& zParameter,
          MyParameter& powerParameter,
          HostInfo& hostInfo);
    // puts out midi into in midi buffer
    void process(juce::AudioBuffer<float>& audioBuffer, juce::MidiBuffer& midi, int length);
    ProcessorCommunicators& getCommunicators();
    ArpPattern arpPattern;
    private:
    MidiScheduler midiScheduler;
    PhaseSignal phaseSignal;
    HostInfo& hostInfo;
    // really should be last -------------------------
    ProcessorCommunicators communicators;
};
