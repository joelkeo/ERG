/*
  ==============================================================================

    BigKnob.cpp
    Created: 8 Feb 2025 6:54:06pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "BigKnob.h"
BigKnob::BigKnob(EditorInfo& editorInfo,
                 MyParameter& parameter) : editorInfo(editorInfo),
parameter(parameter),
attachment(parameter, *this) {
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    attachment.sendInitialUpdate();
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    setRange(0.0f, 1.0f, parameter.getInterval());
    DBG(parameter.getParameterID());
    DBG("SET INTERVAL TO THIS: " << getInterval());
}

void BigKnob::paint(juce::Graphics& g) {
    g.fillAll(juce::Colour(255,181,237));
    juce::Colour bnc = juce::Colour(188,49,169);
    float height = getHeight();
    float width = getWidth();
    // DBG("VALUE: " << getValue());
    double minValue = getMinimum();
    double maxValue = getMaximum();
    double sliderValue = getValue();
    double normalizedValue = (sliderValue - minValue) / (maxValue - minValue);
    float rectHeight = normalizedValue * height;
    float topY = height - normalizedValue * height;
    g.setColour(editorInfo.tertiary());
    juce::Rectangle<int> box(0, topY, width, rectHeight);
    g.setColour(bnc.withAlpha(.4f));
    g.drawRect(box, 1);
    g.drawRect(getLocalBounds());
    g.setColour(bnc.withAlpha(.4f));
    g.fillRect(box);
    /// FONT
    g.setFont(juce::Font(16.f));
    g.setColour(editorInfo.secondary());
    juce::String str = parameter.name + "\n" + parameter.getCurrentValueAsText();
    g.drawFittedText(str, 0, 0, width, height, juce::Justification::centred, 2);
    // BORDER
    g.setColour(juce::Colour(102,74,97));
    g.drawRect(getLocalBounds(), 1);
}
