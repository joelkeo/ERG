/*
  ==============================================================================

    ArpPatternComponent.cpp
    Created: 6 Feb 2025 11:57:37am
    Author:  Joel Keohane

  ==============================================================================
*/

#include "ArpPatternComponent.h"
ArpPatternComponent::ArpPatternComponent(EditorInfo& editorInfo,
                                         ProcessorCommunicators& communicators) :
editorInfo(editorInfo),
pattern(communicators.arpPattern){
    startTimer(1000 / 60);
}

void ArpPatternComponent::paint(juce::Graphics& g) {
    double width = getWidth();
    double height = getHeight();
    double patternLength = pattern.getLength();
    int patternHeight = pattern.getHeight();
    int singleNoteHeight = height / patternHeight;
    // HORIZANTAL LINES
    g.setColour(editorInfo.tertiary().withAlpha(.7f));
    for (int i = 0; i <= patternHeight; i++) {
        int y = i * singleNoteHeight;
        g.drawLine(0, y, width, y);
    }
    // VERTICAL LINES
    g.setColour(editorInfo.border().withAlpha(.5f));
    int halfWidth = width /2;
    g.drawLine(halfWidth, 0, halfWidth, height);
    g.setColour(editorInfo.border().withAlpha(.3f));
    int quarterWidth = width / 4;
    g.drawLine(quarterWidth, 0, quarterWidth, height);
    g.drawLine(3 * quarterWidth, 0, 3 * quarterWidth, height);
    // NOTES
    int patternPosition = pattern.getPosition();
    for (ArpNote& note : pattern.getNotes()) {
        int noteCurrentStart = note.currentStart;
        int noteCurrentEnd = note.currentEnd;
        int noteStartX = (((double) noteCurrentStart) / patternLength) * width;
        int noteLength = noteCurrentEnd - noteCurrentStart;
        int noteWidth = (((double) noteLength) / ((double) patternLength)) * width;
        int noteStartY = height - (singleNoteHeight * (note.noteNum + 1));
        if (noteCurrentStart <= patternPosition && noteCurrentEnd >= patternPosition) {
            g.setColour(editorInfo.tertiary().withAlpha(.5f));
            g.drawRect(noteStartX, noteStartY, noteWidth, singleNoteHeight, 5);
        }
        else {
            g.setColour(editorInfo.secondary().withAlpha(.5f));
            g.drawRect(noteStartX, noteStartY, noteWidth, singleNoteHeight, 2);
        }
    }
    // POSITION
    g.setColour(editorInfo.highlight());
    int x = (((double) patternPosition) / patternLength) * width;
    if (patternPosition > 0) {
        DBG("Pattern position");
        DBG(patternPosition);
        DBG(x);
    }
    g.drawLine(x, 0, x, height, 2);
}

void ArpPatternComponent::timerCallback() {
    repaint();
}

void ArpPatternComponent::mouseDown(const juce::MouseEvent& e) {
    auto position = e.getPosition();
    double width = getWidth();
    double height = getHeight();
    double x = ((double) position.getX()) / width;
    double y = 1. - ((double) position.getY()) / height;
    selection = pattern.attemptSelection(x, y);
    DBG("MOUSE DOWN");
    if (selection.hasNote) {
        DBG("HAS NOTE");
    }
    DBG(selection.noteNum);
}
void ArpPatternComponent::mouseDrag(const juce::MouseEvent& e) {
    if (! selection.hasNote) {return;}
    double width = getWidth();
    double height = getHeight();
    auto position = e.getPosition();
    double x = ((double) position.getX()) / width;
    double y = 1. - ((double) position.getY()) / height;
    double xDelta = ((double) e.getDistanceFromDragStartX()) / width;
    pattern.moveNote(selection, x, y, xDelta);
}
void ArpPatternComponent::mouseUp(const juce::MouseEvent& e) {
    if (! selection.hasNote) {return;}
    pattern.releaseNote(selection);
    selection.hasNote = false;
}

void ArpPatternComponent::mouseDoubleClick(const juce::MouseEvent& e) {
    auto position = e.getPosition();
    double width = getWidth();
    double height = getHeight();
    double x = ((double) position.getX()) / width;
    double y = 1. - ((double) position.getY()) / height;
    pattern.addNote(x, y);
    pattern.attemptSelection(x, y);
}
