/*
  ==============================================================================

    PhaseSignalEvent.h
    Created: 31 Jan 2025 12:40:51pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
class PhaseSignalEvent {
    public:
    PhaseSignalEvent(int position, bool turnOn) {
        this->position = position;
        this->turnOn = turnOn;
    }
    int position;
    bool turnOn;
};
