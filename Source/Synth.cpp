/*
  ==============================================================================

    Synth.cpp
    Created: 31 Jan 2025 11:51:58am
    Author:  Joel Keohane

  ==============================================================================
*/

#include "Synth.h"

Synth::Synth(juce::AudioParameterFloat& lengthParameter,
             juce::AudioParameterFloat& endRateParameter,
             juce::AudioParameterInt& zParameter,
             juce::AudioParameterFloat& powerParameter,
             HostInfo& hostInfo) :
arpPattern(hostInfo),
phaseSignal(lengthParameter, endRateParameter, zParameter, powerParameter, hostInfo),
hostInfo(hostInfo),
communicators(phaseSignal.attachment,
              arpPattern){}

void Synth::process(juce::AudioBuffer<float>& audioBuffer, juce::MidiBuffer& midi, int length) {
    // clear so it can be used as midi out
    std::vector<PhaseSignalEvent> events = midiScheduler.loadNewMidi(midi, length);
    midi.clear();
    double* phaseSignalBuffer = phaseSignal.process(events, length);
    /*
    // testing phase by putting it to audio
    float* a = audioBuffer.getWritePointer(0);
    for (int i = 0; i < length; i++) {
        a[i] = std::max(0., phaseSignalBuffer[i]);
    }
    */
    juce::MidiBuffer& arpMidi = arpPattern.process(phaseSignalBuffer, length);
    midiScheduler.process(arpMidi, midi);
}
ProcessorCommunicators& Synth::getCommunicators() {
    return communicators;
}
