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


ArpNote::ArpNote(juce::var json) {
    auto object = json.getDynamicObject();
    this->noteNum = object->getProperty("noteNum");
    this->currentStart = object->getProperty("currentStart");
    this->currentEnd = object->getProperty("currentEnd");
    this->stableStart = object->getProperty("stableStart");
    this->stableEnd = object->getProperty("stableEnd");
    this->velocity = object->getProperty("velocity");
}

juce::var ArpNote::getJSON() {
    auto object = new juce::DynamicObject();
    object->setProperty("noteNum", this->noteNum);
    object->setProperty("currentStart", this->currentStart);
    object->setProperty("currentEnd", this->currentEnd);
    object->setProperty("stableStart", this->stableStart);
    object->setProperty("stableEnd", this->stableEnd);
    object->setProperty("velocity", this->velocity);
    return juce::var(object);
}
