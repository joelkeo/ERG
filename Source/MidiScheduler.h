/*
  ==============================================================================

    MidiScheduler.h
    Created: 31 Jan 2025 12:10:21pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PhaseSignalEvent.h"
class MidiScheduler {
    public:
    // handles transition from old ranges to new and new notes
    // kind of "expensive", but we expect VERY FEW actual input midi messages
    // just want to be able to handle all cases
    std::vector<PhaseSignalEvent>  loadNewMidi(juce::MidiBuffer& midi, int length);
    juce::MidiBuffer& process(juce::MidiBuffer& arpSignal, juce::MidiBuffer& outBuffer);
    private:
    void handleNoteOn(int note, float velocity, int position);
    void handleNoteOff(int note, int position);
    const int channelNumber = 1;
    bool on = false;
    int note;
    std::set<int> noteSet;
};
