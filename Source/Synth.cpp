/*
  ==============================================================================

    Synth.cpp
    Created: 31 Jan 2025 11:51:58am
    Author:  Joel Keohane

  ==============================================================================
*/

#include "Synth.h"

Synth::Synth(MyParameter& lengthParameter,
             MyParameter& endRateParameter,
             MyParameter& zParameter,
             MyParameter& powerParameter,
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
    // HANDLING DANGLING NOTES:::::
    juce::MidiBuffer& arpMidi = arpPattern.process(phaseSignalBuffer, length);
    for (PhaseSignalEvent event : events) {
        std::set<int> noteOffs = event.noteOffSet;
        int position = event.position;
        for (int noteNum : noteOffs) {
            midi.addEvent(juce::MidiMessage::noteOff(1, noteNum, 1.0f), position);
        }
    }
    midiScheduler.process(arpMidi, midi);
}
ProcessorCommunicators& Synth::getCommunicators() {
    return communicators;
}
