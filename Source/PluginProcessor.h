/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ExponentialPhaseSignal.h"
#include "Synth.h"
#include "MyParameter.h"

//==============================================================================
/**
*/
using APF = juce::AudioParameterFloat;
using API = juce::AudioParameterInt;
using APC = juce::AudioParameterChoice;
using PID = juce::ParameterID;
class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    // Parameter
    MyParameter* lengthParameter = new MyParameter("length",
                                                   "Length",
                                                   [](float x){return std::pow(2, 6 * x);},
                                                   [](float x){return juce::String(x) + " beats";},
                                                   .3333333f,
                                                   6);
    MyParameter* endRateParameter = new MyParameter("endRate",
                                                   "End Rate",
                                                   [](float x){return .125f * std::pow(2.f, 9.f*x);},
                                                   [](float x){return "*" + juce::String(x);},
                                                    .22222222f,
                                                    9);
    MyParameter* zParameter = new MyParameter("z",
                                              "oscillations",
                                              [](float x){return 1.f + x * 99.f;},
                                              [](float x){return juce::String(x) + " oscillations";},
                                              .1111111f,
                                              99);
    MyParameter* powerParameter = new MyParameter("power",
                                                   "power",
                                                   [](float x){return 1.f + x * 4.f;},
                                                   [](float x){return "x^" + juce::String(x);},
                                                  .25f,
                                                  64);
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
    Synth synth;
    HostInfo hostInfo;
};
