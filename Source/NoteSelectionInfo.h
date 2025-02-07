/*
  ==============================================================================

    NoteSelectionInfo.h
    Created: 6 Feb 2025 2:24:58pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
class NoteSelectionInfo {
    public:
    NoteSelectionInfo(int noteNum, bool isEnd) {
        this->noteNum = noteNum;
        this->isEnd = isEnd;
        hasNote = true;
    }
    NoteSelectionInfo() {hasNote = false;}
    int noteNum;
    bool isEnd;
    bool hasNote = false;
};
