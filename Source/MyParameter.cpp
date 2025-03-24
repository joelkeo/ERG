/*
  ==============================================================================

    MyParameter.cpp
    Created: 22 Mar 2025 4:22:18pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "MyParameter.h"
MyParameter::MyParameter(juce::String parameterId,
                         juce::String name,
                         const std::function<float(float)> valueFunction,
                         const std::function<juce::String(float)> stringFunction,
                         float defaultVal,
                         int steps)
: juce::AudioParameterFloat(juce::ParameterID{parameterId,1}, name, juce::NormalisableRange<float> (0.0f, 1.0f), defaultVal),
valueFunction(valueFunction),
stringFunction(stringFunction),
steps(steps) {}

float MyParameter::getAdjustedValue() {
    return valueFunction(get());
}

juce::String MyParameter::getCurrentValueAsText() const {
    float val = valueFunction(get());
    return stringFunction(val);
}

float MyParameter::getInterval() {
    if (steps == 0) {
        return 0.0f;
    }
    else {
        return 1.0f / ((float) steps);
    }
}
