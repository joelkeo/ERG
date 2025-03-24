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
    g.fillAll(juce::Colour(249,196,238));
    juce::Colour noteColor = editorInfo.secondary();
    juce::Colour noteHighlightedColor = editorInfo.secondary();
    juce::Colour verticalLinesColor = juce::Colour(58,42,51);
    juce::Colour horizantalLinesColor = editorInfo.border();
    juce::Colour shadedBarsColor = juce::Colour(251,180,236);
    double width = getWidth();
    double height = getHeight();
    double patternLength = pattern.getLength();
    int patternHeight = pattern.getHeight();
    int singleNoteHeight = height / patternHeight;
    // SHADINGS
    g.setColour(shadedBarsColor.withAlpha(.8f));
    for (int i = 0; i <= patternHeight; i+=2) {
        int y = i * singleNoteHeight;
        g.fillRect(0, y, width, singleNoteHeight);
    }
    // HORIZANTAL LINES
    g.setColour(horizantalLinesColor.withAlpha(.7f));
    for (int i = 0; i <= patternHeight; i++) {
        int y = i * singleNoteHeight;
        g.drawLine(0, y, width, y);
    }
    // VERTICAL LINES
    int powers = 10;
    for (int power = 0; power < powers; power++) {
        g.setColour(verticalLinesColor.withAlpha((float) std::pow(.5f, power) * .7f));
        int numLines = std::pow(2, power);
        float c = width / numLines;
        for (int line = 0; line < numLines; line+= 2) {
            int xPos = c * line;
            g.drawLine(xPos, 0, xPos, height);
        }
    }
    // TESTING OUT LABELLING 1 BETA
    g.setColour(juce::Colour(196,100,172).withAlpha(.8f));
    g.setFont(juce::Font(12.f));
    g.drawText("1 beat", getLocalBounds().reduced(2).removeFromRight(90), juce::Justification::topRight);
    g.setColour(juce::Colour(196,100,172).withAlpha(.6f));
    g.setFont(juce::Font(8.f));
    g.drawText("1/2 beat", getLocalBounds().removeFromLeft(width / 2).reduced(2).removeFromRight(90), juce::Justification::topRight);
    // whhhhh
    int halfWidth = width /2;
    g.drawLine(halfWidth, 0, halfWidth, height);
    g.setColour(verticalLinesColor.withAlpha(.2f));
    int quarterWidth = width / 4;
    g.drawLine(quarterWidth, 0, quarterWidth, height);
    g.drawLine(3 * quarterWidth, 0, 3 * quarterWidth, height);
    // NOTES
    g.setFont(juce::Font(16.f));
    int patternPosition = pattern.getPosition();
    for (ArpNote& note : pattern.getNotes()) {
        int noteCurrentStart = note.currentStart;
        int noteCurrentEnd = note.currentEnd;
        int noteStartX = (((double) noteCurrentStart) / patternLength) * width;
        int noteLength = noteCurrentEnd - noteCurrentStart;
        int noteWidth = (((double) noteLength) / ((double) patternLength)) * width;
        int clippedNoteNum = std::min(note.noteNum, 4);
        int noteStartY = height - (singleNoteHeight * (clippedNoteNum + 1));
        juce::Rectangle<int> noteBounds(noteStartX, noteStartY, noteWidth, singleNoteHeight);
        // BEING PLAYED TODO: better logic for this so I don't MISS notes
        if (noteCurrentStart <= patternPosition && noteCurrentEnd >= patternPosition) {
            g.setColour(noteHighlightedColor.withAlpha(.15f));
            g.fillRect(noteStartX, noteStartY, noteWidth, singleNoteHeight);
            g.setColour(noteHighlightedColor.withAlpha(.99f));
            g.drawRect(noteBounds, 2);
        }
        else {
            g.setColour(noteColor.withAlpha(.075f));
            g.fillRect(noteBounds);
            g.setColour(noteColor.withAlpha(.87f));
            g.drawRect(noteBounds, 2);
        }
        g.drawText(noteNumToString(note.noteNum), noteBounds, juce::Justification::centred);
        // DBG("DRAWING NOTE OF NUM: " << note.noteNum);
        // BORDER
        g.setColour(juce::Colour(102,74,97));
        g.drawRect(getLocalBounds(), 1);
    }
    // POSITION
    g.setColour(editorInfo.highlight().withAlpha(.7f));
    double x = (((double) patternPosition) / patternLength) * width;
    g.drawLine(x, 0.f, x, height, 2.f);
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
    // attempts to select a note, if one is there, delete it
    NoteSelectionInfo attemptedSelection = pattern.attemptSelection(x, y);
    if (attemptedSelection.hasNote) {
        DBG("selection HAD IT");
        pattern.deleteNote(attemptedSelection.noteNum);
        selection.hasNote = false;
        return;
    }
    pattern.addNote(x, y);
    // will select the newly added note
    pattern.attemptSelection(x, y);
}

juce::String ArpPatternComponent::noteNumToString(int num) {
    if (num == 0) {
        return "All";
    }
    else {
        return juce::String(num);
    }
}
