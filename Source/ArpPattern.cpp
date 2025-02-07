/*
  ==============================================================================

    ArpPattern.cpp
    Created: 31 Jan 2025 12:09:23pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "ArpPattern.h"
ArpPattern::ArpPattern(HostInfo& hostInfo) : hostInfo(hostInfo) {
    // hardcode to 1 beat(s) now
    int beats = 1;
    patternLength = ((double) beats) * resolution;
    DBG("pattern length");
    DBG(patternLength);
    this->addNote(0, 0);
}

juce::MidiBuffer& ArpPattern::process(double* phaseSignal, int length) {
    outBuffer.clear();
    for (int i = 0; i < length; i++) {
        // go from ceiling of previous to floor of current
        double currentPhase = phaseSignal[i];
        int right = std::floor(currentPhase * patternLength);
        if (right < 0) {}
        else if (left < 0) {
            // only floor of current
            if (hasEventBuffer.test(right)) {
                getNotesAt(outBuffer, right, i);
            }
        }
        else if (left - 1 > right) {
            // wrap around case:  previous to end
            for (int leftMoving = left; leftMoving < patternLength; leftMoving++) {
                if (! hasEventBuffer.test(leftMoving)) {continue;}
                getNotesAt(outBuffer, leftMoving, i);
            }
            // then begining to current
            for (int leftMoving = 0; leftMoving <= right; leftMoving++) {
                if (! hasEventBuffer.test(leftMoving)) {continue;}
                getNotesAt(outBuffer, leftMoving, i);
            }
        }
        else {
            // everything between ceilling of previous and floor of current
            for (int leftMoving = left; leftMoving <= right; leftMoving++) {
                if (! hasEventBuffer.test(leftMoving)) {continue;}
                getNotesAt(outBuffer, leftMoving, i);
            }
        }
        left = right + 1;
        this->position = left;
    }
    return outBuffer;
}

void ArpPattern::getNotesAt(juce::MidiBuffer& midi, int arpIndex, int outIndex) {
    DBG("notes detected");
    juce::MidiBufferIterator iterator = messages.findNextSamplePosition(arpIndex);
    juce::MidiBufferIterator end = messages.end();
    while (iterator != end) {
        juce::MidiMessage currentMessage = (*iterator).getMessage();
        if (currentMessage.getTimeStamp() != arpIndex) {return;}
        else {
            DBG("added midi event");
            midi.addEvent(currentMessage, outIndex);
        }
        ++iterator;
    }
}

void ArpPattern::addMessage(juce::MidiMessage midiMessage, int i) {
    messages.addEvent(midiMessage, i);
    hasEventBuffer.set(i);
}

void ArpPattern::addNote(double normalizedX, double normalizedY) {
    int start = normalizedX * patternLength;
    int end = std::min(start + defaultSize, patternLength - 1);
    int noteNum = normalizedYToNoteNum(normalizedY);
    ArpNote note = ArpNote(start, end, noteNum, defaultVelocity);
    notes.push_back(note);
    resetMessages();
}

void ArpPattern::moveNote(NoteSelectionInfo selection, double normalizedX, double normalizedY,
                          double xDelta) {
    int noteIndex = selection.noteNum;
    ArpNote& note = notes[noteIndex];
    if (selection.isEnd) {
        int unrangedNewEnd = normalizedX * patternLength;
        int rangedNewEnd = std::min(std::max(unrangedNewEnd, note.currentStart), patternLength - 1);
        note.currentEnd = rangedNewEnd;
    }
    else {
        int newNoteNum = normalizedYToNoteNum(normalizedY);
        int scaledDelta = xDelta * patternLength;
        int unrangedNewStart = note.stableStart + scaledDelta;
        int unrangedNewEnd = note.stableEnd + scaledDelta;
        int rangedNewStart = std::min(std::max(unrangedNewStart, 0), patternLength - 2);
        int rangedNewEnd = std::min(std::max(unrangedNewEnd, rangedNewStart), patternLength - 1);
        note.currentStart = rangedNewStart;
        note.currentEnd = rangedNewEnd;
        note.noteNum = newNoteNum;
    }
    resetMessages();
}

void ArpPattern::releaseNote(NoteSelectionInfo selection) {
    notes[selection.noteNum].stabilize();
}

void ArpPattern::resetMessages() {
    messages.clear();
    hasEventBuffer.reset();
    for (ArpNote& note : notes) {
        int noteCurrentStart = note.currentStart;
        int noteCurrentEnd = note.currentEnd;
        messages.addEvent(juce::MidiMessage::noteOn(channelNumber, note.noteNum, note.velocity), noteCurrentStart);
        messages.addEvent(juce::MidiMessage::noteOff(channelNumber, note.noteNum, note.velocity),
                          note.currentEnd);
        hasEventBuffer.set(noteCurrentStart);
        hasEventBuffer.set(noteCurrentEnd);
    }
}

int ArpPattern::normalizedYToNoteNum(double y) {
    return std::max(0., std::floor(y * yHeight));
}

int ArpPattern::getLength() {
    return patternLength;
}

int ArpPattern::getHeight() {
    return yHeight;
}

int ArpPattern::getPosition() {
    return position;
}

std::vector<ArpNote> ArpPattern::getNotes() {
    return notes;
}

NoteSelectionInfo ArpPattern::attemptSelection(double normalizedX, double normalizedY) {
    int noteNum = normalizedYToNoteNum(normalizedY);
    int scaledX = normalizedX * patternLength;
    for (int i = 0; i < notes.size(); i++) {
        ArpNote& note = notes[i];
        int endPrecision = 1;
        // first consider note end
        if (note.noteNum == noteNum && endPrecision >= std::abs(scaledX - note.currentEnd)) {
            return NoteSelectionInfo(i, true);
        }
        if (note.noteNum == noteNum && scaledX > note.currentStart && scaledX < note.currentEnd) {
            return NoteSelectionInfo(i, false);
        }
    }
    return NoteSelectionInfo();
}
