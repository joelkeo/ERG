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
    double width = getWidth();
    double height = getHeight();
    // HANDLE PATH SCALING
    juce::AffineTransform scaleTransform = juce::AffineTransform::scale(width, height);
    ratePath.applyTransform(scaleTransform);
    // COLORS
    juce::Colour backgroundColor = editorInfo.main();
    juce::Colour phaseColor = editorInfo.secondary();
    juce::Colour phaseBoxColor = editorInfo.secondary();
    juce::Colour rateColor = juce::Colour(77,93,91);
    g.fillAll(juce::Colour(242,240,241));
    // DRAW RATE LINE
    juce::PathStrokeType strokeType(1.0f, juce::PathStrokeType::curved);
    g.setColour(rateColor);
    g.strokePath(ratePath, strokeType);
    // FILL IN RATE PATH PORTION
    g.saveState();
    g.reduceClipRegion(0, 0, position, getHeight());
    g.setColour(juce::Colour(235,163,217).withAlpha(.3f));
    g.fillPath(ratePath);
    g.restoreState();
    // DRAW PHASE LINES
    g.setColour(phaseColor.withAlpha(.3f));
    double halfHeight = height / 2.;
    for (int i = 0; i < lineXPositions.size(); i++) {
        double x = lineXPositions[i] * width;
        double yScaling = lineYScalings[i];
        double yStart =  halfHeight * (1. - yScaling);
        g.drawLine(x, yStart, x, yStart + height * yScaling);
    }
    // HIGHLIGHT CURRENT PHASE PATTERN
    if (showingPosition && zIndex < lineXPositions.size()) {
        double xEnd = zIndex >= lineXPositions.size() - 1 ? width : lineXPositions[zIndex+1] * width;
        int xStart = lineXPositions[zIndex] * width;
        double yScaling = lineYScalings[zIndex];
        double yStart =  halfHeight * (1. - yScaling);
        double patternWidth = xEnd - xStart;
        float alphaStart = .8f;
        float alpha = (1.f - this->moddedValue) * alphaStart;
        g.setColour(phaseBoxColor.withAlpha(alpha));
        g.fillRect(xStart, yStart, patternWidth, height * yScaling);
        g.setColour(rateColor);
        g.drawLine(position, 0, position, height, 1);
    }
    // AXIS LABELLING::
    double yPosDelta = height / (maxRate / labelDelta);
    for (double pos = 0; pos < maxRate; pos += labelDelta) {
        double y = height - yPosDelta * pos;
        g.drawLine(width - 10, y, width, y, 5);
        DBG("axis y");
        DBG(y);
        DBG("y pos delta");
        DBG(yPosDelta);
        DBG("label delta");
        DBG(labelDelta);
        DBG("max rate");
        DBG(maxRate);
    }
    // UNDO SCALING
    juce::AffineTransform inverseTransform = juce::AffineTransform::scale(1 / width, 1 / height);
    ratePath.applyTransform(inverseTransform);
}

// position is 0,1 normalized
void EnvelopeComponent::updatePosition(double position, int zIndex, double moddedValue) {
    this->zIndex = zIndex;
    this->moddedValue = moddedValue;
    showingPosition = true;
    this->position = getWidth() * position;
}
void EnvelopeComponent::clearPosition() {
    showingPosition = false;
}
void EnvelopeComponent::updateFormula(std::function<double(double)> phaseFormula,
              std::function<double(double)> rateFormula,
                                      int length) {
    // RATE FORMULA
    double maxY = std::max(rateFormula(0.), rateFormula(length));
    double yScaling = 1 / maxY;
    double maxX = length;
    double xScaling = 1 / maxX;
    int resolution = 100;
    double xIncrement = ((double) length) / resolution;
    ratePath.clear();
    // PATH START
    double y = rateFormula(0);
    ratePath.startNewSubPath(0 * xScaling, 1 - y * yScaling);
    for (double x = 0.; x < length; x += xIncrement) {
        double y = rateFormula(x);
        ratePath.lineTo(x * xScaling, 1 - y * yScaling);
    }
    double lastY = rateFormula(length) * yScaling;
    if (std::isnan(lastY)) {
        lastY = 1.;
    }
    ratePath.lineTo(length * xScaling, 1 - lastY);
    ratePath.lineTo(length * xScaling, 1);
    ratePath.lineTo(0, 1);
    ratePath.closeSubPath();
    // PHASE FORMULA
    phasePath.clear();
    // pATH START
    y = phaseFormula(0);
    phasePath.startNewSubPath(0 * xScaling, 1 - y);
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
    // AXIS LABELLING:: (determine distance between labels)
    double rateDelta = .125;
    while (maxY / rateDelta > 8) {
        rateDelta *= 2.;
    }
    this->labelDelta = rateDelta;
    this->maxRate = maxY;
    DBG("Max y");
    DBG(maxRate);
    DBG("rate delta");
    DBG(labelDelta);
}
void EnvelopeComponent::timerCallback() {
    repaint();
}
