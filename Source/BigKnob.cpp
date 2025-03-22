/*
  ==============================================================================

    BigKnob.cpp
    Created: 8 Feb 2025 6:54:06pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "BigKnob.h"
BigKnob::BigKnob(EditorInfo& editorInfo,
                 juce::RangedAudioParameter& parameter) : editorInfo(editorInfo),
parameter(parameter),
attachment(parameter, *this) {
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    attachment.sendInitialUpdate();
}

void BigKnob::paint(juce::Graphics& g) {
    /*
    // scaling
    double height = getHeight();
    double width = getWidth();
    double minDimension = std::min(height, width);
    double x = std::abs(width - minDimension) / 2;
    double y = std::abs(height - minDimension) / 2;
    height = minDimension;
    width = minDimension;
    double rotaryStartAngle = 3.665;
    double rotaryEndAngle = 8.792;
    double minValue = getMinimum();
    double maxValue = getMaximum();
    double sliderValue = getValue();
    double sliderPosProportional = (sliderValue - minValue) / (maxValue - minValue);
    x += 15;
    width -= 30;
    auto radiusInner = (float) juce::jmin(width / 2, height / 2);
    auto radiusOuter = radiusInner + 3.0f;
    auto centerX = (float) x + (float) width * 0.5f;
    auto centerY = (float) y + (float) height * 0.5f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    // Draw the outline of the rotary track
    g.setColour(editorInfo.secondary()); // Customize the color as needed
    juce::Path trackPath;
    trackPath.addCentredArc(centerX, centerY, radiusInner, radiusInner, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    juce::Path strokedTrackPath;
    juce::PathStrokeType stroke(8.0f);
    stroke.setEndStyle(juce::PathStrokeType::EndCapStyle::rounded);
    stroke.createStrokedPath(strokedTrackPath, trackPath);
    g.strokePath(strokedTrackPath, juce::PathStrokeType(1.0f));
    // Draw the rotary thumb
    juce::Path thumb;
    auto thumbWidth = 8.0f;
    thumb.addEllipse(-thumbWidth / 2, -radiusOuter - 1.0f, thumbWidth, thumbWidth);
    g.setColour(editorInfo.secondary());
    g.fillPath(thumb,
               juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    // TEXT
    g.setFont(juce::Font(20.f));
    g.setColour(editorInfo.tertiary());
    juce::String str = parameter.name + "\n" + parameter.getCurrentValueAsText();
    g.drawFittedText(str, x, y, width, height, juce::Justification::centred, 2);
     */
    g.fillAll(juce::Colour(242,240,241));
    juce::Colour bnc = juce::Colour(221,77,196);
    float height = getHeight();
    float width = getWidth();
    DBG("VALUE: " << getValue());
    double minValue = getMinimum();
    double maxValue = getMaximum();
    double sliderValue = getValue();
    double normalizedValue = (sliderValue - minValue) / (maxValue - minValue);
    float rectHeight = normalizedValue * height;
    float topY = height - normalizedValue * height;
    g.setColour(editorInfo.tertiary());
    juce::Rectangle<int> box(0, topY, width, rectHeight);
    g.setColour(bnc.withAlpha(.3f));
    g.drawRect(box, 1);
    g.drawRect(getLocalBounds());
    g.setColour(bnc.withAlpha(.3f));
    g.fillRect(box);
    /// FONT
    g.setFont(juce::Font(20.f));
    g.setColour(bnc);
    juce::String str = parameter.name + "\n" + parameter.getCurrentValueAsText();
    g.drawFittedText(str, 0, 0, width, height, juce::Justification::centred, 2);
}
