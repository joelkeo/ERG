/*
  ==============================================================================

    MyParameter.h
    Created: 22 Mar 2025 4:22:18pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class MyParameter : public juce::AudioParameterFloat {
    // new APF(PID{"length",1}, "length", 1.f, 64.f, 8.f);
    public:
    MyParameter(juce::String parameterId,
                juce::String name,
                const std::function<float(float)> valueFunction,
                const std::function<juce::String(float)> stringFunction,
                float defaultVal,
                int steps = 0);
    // IMPORTANT: USE THIS INSTEAD OF .get() or (float)
    float getAdjustedValue();
    juce::String getCurrentValueAsText() const override;
    float getInterval();
    private:
    const std::function<float(float)> valueFunction;
    const std::function<juce::String(float)> stringFunction;
    int steps;
};
