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
class Synth {
    public:
    Synth(juce::AudioParameterFloat& lengthParameter,
          juce::AudioParameterFloat& endRateParameter,
          juce::AudioParameterInt& zParameter,
          juce::AudioParameterFloat& powerParameter,
          HostInfo& hostInfo);
    // puts out midi into in midi buffer
    void process(juce::AudioBuffer<float>& audioBuffer, juce::MidiBuffer& midi, int length);
    ProcessorCommunicators& getCommunicators();
    private:
    MidiScheduler midiScheduler;
    ArpPattern arpPattern;
    PhaseSignal phaseSignal;
    HostInfo& hostInfo;
    // really should be last -------------------------
    ProcessorCommunicators communicators;
};
