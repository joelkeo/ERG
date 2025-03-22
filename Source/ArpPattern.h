/*
  ==============================================================================

    ArpPattern.h
    Created: 31 Jan 2025 12:09:23pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "HostInfo.h"
#include "ArpNote.h"
#include "NoteSelectionInfo.h"
class ArpPattern {
    public:
    ArpPattern(HostInfo& hostInfo);
    juce::MidiBuffer& process(double* phaseSignal, int lenght);
    void getNotesAt(juce::MidiBuffer& midi, int arpInex, int outIndex);
    void addNote(double normalizedX, double normalizedY);
    void moveNote(NoteSelectionInfo selection, double normalizedX, double normalizedY, double xDelta);
    void releaseNote(NoteSelectionInfo selection);
    void deleteNote(int noteNum);
    NoteSelectionInfo attemptSelection(double normalizedX, double normalizedY);
    int getLength();
    int getHeight();
    int getPosition();
    std::vector<ArpNote> getNotes();
    private:
    // messages in the arp buffer
    juce::MidiBuffer messages;
    std::vector<ArpNote> notes;
    // output midi buffer
    juce::MidiBuffer outBuffer;
    int left = -1;
    int patternLength;
    private:
    HostInfo& hostInfo;
    // IMPORT STUFF
    int position = 0;
    const int channelNumber = 1;
    const static int resolution = 128;
    const static int maxLengthInBeats = 8;
    int yHeight = 5;
    int defaultSize = resolution / 4;
    float defaultVelocity = .8f;
    void addMessage(juce::MidiMessage midiMessage, int location);
    int normalizedYToNoteNum(double y);
    std::vector<std::uint8_t> messageCounts;
    std::bitset<resolution * (maxLengthInBeats + 1)> hasEventBuffer;
    void resetMessages();
    int CLIP_HEIGHT = 4;
    int clipNoteNum(int noteNum);
};
