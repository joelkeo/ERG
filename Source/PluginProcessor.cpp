/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
synth(*lengthParameter, *endRateParameter, *zParameter, *powerParameter, hostInfo)
#endif
{
    addParameter(lengthParameter);
    addParameter(endRateParameter);
    addParameter(zParameter);
    addParameter(powerParameter);
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    hostInfo.sampleRate = sampleRate;
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    if (! midiMessages.isEmpty()) {
        DBG("num events:");
        DBG(midiMessages.getNumEvents());
    }
    
    if (auto bpmFromHost = *getPlayHead()->getPosition()->getBpm()) {
        hostInfo.bpm = bpmFromHost;
    }
    synth.process(buffer, midiMessages, buffer.getNumSamples());
    // DBG("FPN: " << juce::File::getSpecialLocation(juce::File::commonDocumentsDirectory).getFullPathName());
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this, synth.getCommunicators());
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::DynamicObject::Ptr jsonState = new juce::DynamicObject();
    // SECTION 1: REGISTERED PARAMETERS ------------------------------
    for (auto parameter : getParameters()) {
        auto* idParameter = dynamic_cast<juce::AudioProcessorParameterWithID*>(parameter);
        if (idParameter) {
            juce::String name = idParameter->name;
            jsonState->setProperty(name, idParameter->getValue());
        }
    }
    
    // SECTION 2: ARPEGGIATOR PATTERN ---------------------------------
    jsonState->setProperty("arpNoteList", synth.arpPattern.getJSON());
    // BUILD
    juce::var jsonData = juce::var(jsonState.get());
    juce::String jsonString = juce::JSON::toString(jsonData);
    destData.append(jsonString.toRawUTF8(), jsonString.getNumBytesAsUTF8());
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::String jsonString (juce::String::fromUTF8(static_cast<const char*> (data), sizeInBytes));
    juce::var jsonData = juce::JSON::parse(jsonString);
    juce::DynamicObject* jsonState = jsonData.getDynamicObject();
    // SECTION 3: REGISTERED PARAMETERS -----------------------------
    for (auto parameter : getParameters()) {
        auto* idParameter = dynamic_cast<juce::AudioProcessorParameterWithID*>(parameter);
        if (idParameter) {
            juce::String name = idParameter->name;
            float value = jsonState->getProperty(name);
            idParameter->setValue(value);
            idParameter->sendValueChangedMessageToListeners(value);
        }
    }
    // SECTION 2: ARPEGGIATOR PATTERN ---------------------------------
    juce::var arpNoteList = jsonState->getProperty("arpNoteList");
    DBG("ARP NOTE LIST: " << arpNoteList.toString());
    synth.arpPattern.setToJSON(arpNoteList);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
