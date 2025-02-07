/*
  ==============================================================================

    MidiScheduler.cpp
    Created: 31 Jan 2025 12:10:32pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "MidiScheduler.h"
std::vector<PhaseSignalEvent> MidiScheduler::loadNewMidi(juce::MidiBuffer &midi, int length) {
    if (! midi.isEmpty()) { DBG("loadNewMidi");}
    for (const juce::MidiMessageMetadata metadata : midi) {
        DBG("INPUT MIDI EVENT");
        juce::MidiMessage currentMessage = metadata.getMessage();
        note = currentMessage.getNoteNumber();
        if (currentMessage.isNoteOn()) {
            noteSet.insert(note);
        }
        else {
            noteSet.erase(note);
        }
    }
    if (! on && ! noteSet.empty()) {
        on = true;
        return std::vector<PhaseSignalEvent>{PhaseSignalEvent(0, true)};
    }
    if (on && noteSet.empty()) {
        on = false;
        return std::vector<PhaseSignalEvent>{PhaseSignalEvent(0, false)};
    }
    return std::vector<PhaseSignalEvent>();
}

// ignore if there is already a live note
void MidiScheduler::handleNoteOn(int note, float velocity, int position) {
    on = true;
    this->note = note;
}

void MidiScheduler::handleNoteOff(int note, int position) {
    on = false;
}

juce::MidiBuffer& MidiScheduler::process(juce::MidiBuffer& arpSignal, juce::MidiBuffer& outBuffer) {
    if (!on) {return outBuffer;}
    int size = noteSet.size();
    for (const juce::MidiMessageMetadata metadata : arpSignal) {
        juce::MidiMessage currentMessage = metadata.getMessage();
        int noteNumber = currentMessage.getNoteNumber();
        // ALL CASE
        if (noteNumber == 0) {
            for (int n : noteSet) {
                juce::MidiMessage newMessage = juce::MidiMessage(currentMessage);
                newMessage.setNoteNumber(n);
                outBuffer.addEvent(newMessage, currentMessage.getTimeStamp());
            }
        }
        // NTH note case
        else {
            juce::MidiMessage newMessage = juce::MidiMessage(currentMessage);
            auto it = std::next(noteSet.begin(), (noteNumber - 1) % size);
            newMessage.setNoteNumber(*it);
            outBuffer.addEvent(newMessage, currentMessage.getTimeStamp());
        }
    }
    return outBuffer;
}
