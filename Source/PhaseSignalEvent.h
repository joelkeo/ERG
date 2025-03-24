/*
  ==============================================================================

    PhaseSignalEvent.h
    Created: 31 Jan 2025 12:40:51pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class PhaseSignalEvent {
    public:
    PhaseSignalEvent(int position, bool turnOn) {
        this->position = position;
        this->turnOn = turnOn;
    }
    PhaseSignalEvent(int position, bool turnOn, std::set<int> noteOffSet) {
        this->position = position;
        this->turnOn = turnOn;
        this->noteOffSet = noteOffSet;
    }
    int position;
    bool turnOn;
    std::set<int> noteOffSet;
};
