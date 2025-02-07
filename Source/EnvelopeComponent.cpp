/*
  ==============================================================================

    EnvelopeComponent.cpp
    Created: 4 Feb 2025 3:02:48pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "EnvelopeComponent.h"
EnvelopeComponent::EnvelopeComponent(EditorInfo& editorInfo,
                                     ProcessorCommunicators& communicators) : attachment(communicators.envelopeAttachment), editorInfo(editorInfo) {
    attachment.addListener(this);
    startTimer(1000 / 60);
}
EnvelopeComponent::~EnvelopeComponent() {
    attachment.removeListener(this);
    stopTimer();
}

void EnvelopeComponent::paint(juce::Graphics& g) {
    DBG("num lines");
    DBG(lineXPositions.size());
    DBG(ratePath.getLength());
    juce::PathStrokeType strokeType(1.0f, juce::PathStrokeType::curved);
    g.fillAll(editorInfo.main());
    g.setColour(editorInfo.secondary().withAlpha(.8f));
    g.strokePath(ratePath, strokeType);
    g.setColour(editorInfo.highlight());
    // FILL IN SHIT
    g.saveState();
    g.reduceClipRegion(0, 0, position, getHeight());
    g.setColour(editorInfo.secondary().withAlpha(.2f));
    g.fillPath(ratePath);
    g.restoreState();
    // END FILL IN SHIT
    g.setColour(editorInfo.tertiary());
    double height = getHeight();
    double halfHeight = height / 2.;
    for (int i = 0; i < lineXPositions.size(); i++) {
        double x = lineXPositions[i];
        double yScaling = lineYScalings[i];
        double yStart =  halfHeight * (1. - yScaling);
        g.drawLine(x, yStart, x, yStart + height * yScaling);
    }
    // HIGHLIGHT CURRENT PATTERN
    if (showingPosition) {
        double xEnd = zIndex >= lineXPositions.size() - 1 ? getWidth() : lineXPositions[zIndex+1];
        int xStart = lineXPositions[zIndex];
        double yScaling = lineYScalings[zIndex];
        double yStart =  halfHeight * (1. - yScaling);
        double patternWidth = xEnd - xStart;
        float alpha = (1.f - this->moddedValue) * .3;
        g.setColour(editorInfo.tertiary().withAlpha(alpha));
        g.fillRect(xStart, yStart, patternWidth, height * yScaling);
        g.setColour(editorInfo.secondary());
        g.drawLine(position, 0, position, height, 1);
    }
}

// position is 0,1 normalized
void EnvelopeComponent::updatePosition(double position, int zIndex, double moddedValue) {
    this->zIndex = zIndex;
    this->moddedValue = moddedValue;
    showingPosition = true;
    this->position = getWidth() * position;
    DBG("updated position");
    DBG(position);
}
void EnvelopeComponent::clearPosition() {
    showingPosition = false;
}
void EnvelopeComponent::updateFormula(std::function<double(double)> phaseFormula,
              std::function<double(double)> rateFormula,
                                      int length) {
    DBG("FORMULA UPDATE -----------------------");
    DBG(length);
    // RATE FORMULA
    int width = getWidth();
    double dWidth = width;
    int height = getHeight();
    double dHeight = height;
    double maxY = std::max(rateFormula(0.), rateFormula(length));
    double yScaling = dHeight / maxY;
    double maxX = length;
    double xScaling = dWidth / maxX;
    int resolution = width;
    double xIncrement = ((double) length) / resolution;
    ratePath.clear();
    // PATH START
    double y = rateFormula(0);
    ratePath.startNewSubPath(0 * xScaling, dHeight - y * yScaling);
    for (double x = 0.; x < length; x += xIncrement) {
        double y = rateFormula(x);
        ratePath.lineTo(x * xScaling, dHeight - y * yScaling);
    }
    ratePath.lineTo(width, height);
    ratePath.lineTo(0, height);
    ratePath.closeSubPath();
    // PHASE FORMULA
    phasePath.clear();
    // pATH START
    y = phaseFormula(0);
    phasePath.startNewSubPath(0 * xScaling, dHeight - y * dHeight);
    double x = .001;
    double addition = .01;
    // BINARY SEARCH
    lineXPositions.clear();
    lineYScalings.clear();
    // LEFTMOST CASE
    lineXPositions.push_back(0);
    lineYScalings.push_back(std::pow(.5, rateFormula(0) * (1. / 32.) * length));
    double targetInt = 1;
    while (x < length) {
        if (phaseFormula(x) >= targetInt) {
            int binaries = 0;
            addition /= 2.;
            x -= addition;
            while (binaries < 10) {
                addition /= 2.;
                if (phaseFormula(x) > targetInt) {
                    x -= addition;
                }
                else {
                    x += addition;
                }
                binaries++;
            }
            // ADD TO LIST
            lineYScalings.push_back(std::pow(.5, rateFormula(x) * (1. / 32.) * length));
            lineXPositions.push_back(x * xScaling);
            addition = .01;
            targetInt += 1.;
        }
        else {
            addition *= 2.;
            x += addition;
        }
    }
    // END BINARY SEARCH
    /*
    for (double x = 0.; x < length; x += xIncrement) {
        double y = std::fmod(phaseFormula(x), 1);
        ratePath.lineTo(x * xScaling, dHeight - y * dHeight);
    }
     */
}
void EnvelopeComponent::timerCallback() {
    repaint();
}
