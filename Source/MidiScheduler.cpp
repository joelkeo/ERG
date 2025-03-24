/*
  ==============================================================================

    MidiScheduler.cpp
    Created: 31 Jan 2025 12:10:32pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "MidiScheduler.h"
std::vector<PhaseSignalEvent> MidiScheduler::loadNewMidi(juce::MidiBuffer &midi, int length) {
    juce::MidiBuffer orderedMidi = orderMessages(midi);
    // TODO: Guarantee invariant that noteoff messages always come first
    std::vector<PhaseSignalEvent> events;
    for (const juce::MidiMessageMetadata metadata : orderedMidi) {
        DBG("INPUT MIDI EVENT");
        juce::MidiMessage currentMessage = metadata.getMessage();
        note = currentMessage.getNoteNumber();
        int position = currentMessage.getTimeStamp();
        if (currentMessage.isNoteOn()) {
            on = true;
            if (noteSet.empty()) {events.push_back(PhaseSignalEvent(position, true));}
            noteSet.insert(note);
            turnOffNoteSet.insert(note);
        }
        else {
            noteSet.erase(note);
            if (noteSet.empty()) {
                events.push_back(PhaseSignalEvent(position, false, turnOffNoteSet));
                on = false;
                turnOffNoteSet.clear();
            }
        }
    }
    return events;
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

juce::MidiBuffer MidiScheduler::orderMessages(juce::MidiBuffer inputBuffer) {
    juce::MidiBuffer outputBuffer;
    int currentTimestamp = -1;
    std::list<juce::MidiMessage> currentTimestampMessages;
    for (const juce::MidiMessageMetadata metadata : inputBuffer) {
        juce::MidiMessage message = metadata.getMessage();
        int position = message.getTimeStamp();
        if (position > currentTimestamp) {
            // LOAD ALL OF THE LINK LIST INTO BUFFER AND CLEAR
            for (juce::MidiMessage message : currentTimestampMessages) {
                outputBuffer.addEvent(message, currentTimestamp);
            }
            currentTimestampMessages.clear();
            currentTimestampMessages.push_back(message);
            currentTimestamp = position;
        }
        else {
            if (message.isNoteOff()) {
                currentTimestampMessages.push_front(message);
            }
            else {
                currentTimestampMessages.push_back(message);
            }
        }
    }
    // HANDLE LEFTOVERS
    for (juce::MidiMessage message : currentTimestampMessages) {
        outputBuffer.addEvent(message, currentTimestamp);
    }
    return outputBuffer;
}
