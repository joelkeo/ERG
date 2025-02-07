/*
  ==============================================================================

    ArpNote.cpp
    Created: 6 Feb 2025 12:44:51pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "ArpNote.h"
ArpNote::ArpNote(int noteStart,
                 int noteEnd,
                 int noteNum,
                 float velocity) {
    this->noteNum = noteNum;
    this->currentStart = noteStart;
    this->currentEnd = noteEnd;
    this->stableStart = noteStart;
    this->stableEnd = noteEnd;
    this->velocity = velocity;
}

void ArpNote::stabilize() {
    this->stableStart = this->currentStart;
    this->stableEnd = this->currentEnd;
}
