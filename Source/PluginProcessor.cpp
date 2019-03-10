/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyArpeggiatorAudioProcessor::MyArpeggiatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	state = new AudioProcessorValueTreeState(*this, nullptr);

	state->createAndAddParameter("octave down", "Octave Down", "Octave Down", NormalisableRange<float>(0.0, 3.0, 1.0), 1.0, nullptr, nullptr);
	state->createAndAddParameter("octave up", "Octave Up", "Octave Up", NormalisableRange<float>(0.0, 3.0, 1.0), 1.0, nullptr, nullptr);
	state->createAndAddParameter("min note length", "Min Note Length", "Min Note Length", NormalisableRange<float>(1.0, 32.0, 1.0), 1.0, nullptr, nullptr);
	state->createAndAddParameter("max note length", "Max Note Length", "Max Note Length", NormalisableRange<float>(1.0, 32.0, 1.0), 1.0, nullptr, nullptr);
	state->createAndAddParameter("meter", "Meter", "Meter", NormalisableRange<float>(0.01, 1.0, 0.01), 1.0, nullptr, nullptr);


	state->state = ValueTree("octave down");
	state->state = ValueTree("octave up");
	state->state = ValueTree("min note length");
	state->state = ValueTree("max note length");
	state->state = ValueTree("meter");

}

MyArpeggiatorAudioProcessor::~MyArpeggiatorAudioProcessor()
{
}

//==============================================================================
const String MyArpeggiatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyArpeggiatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyArpeggiatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyArpeggiatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyArpeggiatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyArpeggiatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyArpeggiatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyArpeggiatorAudioProcessor::setCurrentProgram (int index)
{
}

const String MyArpeggiatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyArpeggiatorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MyArpeggiatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	ignoreUnused(samplesPerBlock);

	notes.clear();
	currentNote = 0;
	lastNoteValue = -1;
	time = 0.0;
	rate = static_cast<float> (sampleRate);

	bpm = 120;
	beatsPerSec = bpm / 60.0;

}

void MyArpeggiatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyArpeggiatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void MyArpeggiatorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	// the audio buffer in a midi effect will have zero channels!
	jassert(buffer.getNumChannels() == 0);

	octaveDownInt = *state->getRawParameterValue("octave down");
	octaveUpInt = *state->getRawParameterValue("octave up");
	minNoteLength = *state->getRawParameterValue("min note length");
	maxNoteLength = *state->getRawParameterValue("max note length");
	meter = *state->getRawParameterValue("meter");

	notesPerBeat = 1.0 / GetClosestMeter(meter);
	notesPerSec = beatsPerSec * notesPerBeat;
	/*
	AudioPlayHead* playHead = getPlayHead();
	if (playHead == nullptr)
	{
		// exit as playhead not found
		return;
	}

	AudioPlayHead::CurrentPositionInfo playheadPositionInfo;
	playHead->getCurrentPosition(playheadPositionInfo);
	*/


	auto numberSamples = 256;// buffer.getNumSamples();
	//auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * 0.1f));
	auto noteDuration = static_cast<int> (std::ceil( rate / notesPerSec )); // eg X samples per note

	MidiMessage midiMessage;
	int samplePosition = 0;

	for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(midiMessage, samplePosition);)
	{
		if (midiMessage.isNoteOn()) notes.add(midiMessage.getNoteNumber());
		else if (midiMessage.isNoteOff()) notes.removeValue(midiMessage.getNoteNumber());
	}

	midiMessages.clear();

//	DBG("Time : " + String(time));
//	DBG("numSamples : " + String(numberSamples));
//	DBG("noteDuration : " + String(noteDuration));

	if ((time + numberSamples) >= noteDuration)
	{
		auto offset = jmax(0, jmin ((int) (noteDuration - time), numberSamples - 1));

		if (lastNoteValue > 0)
		{
			midiMessages.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
			lastNoteValue = -1;
		}

		if (notes.size() > 0)
		{
			currentNote = (currentNote + 1) % notes.size();
			lastNoteValue = notes[currentNote];
			midiMessages.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);
		}
	}

	time = (time + numberSamples) % noteDuration;
}

AudioProcessorValueTreeState& MyArpeggiatorAudioProcessor::getState()
{
	return *state;
}

MidiKeyboardState& MyArpeggiatorAudioProcessor::getMidiKeyState()
{
	return *midiKeyState;
}



//==============================================================================
bool MyArpeggiatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MyArpeggiatorAudioProcessor::createEditor()
{
    return new MyArpeggiatorAudioProcessorEditor (*this);
}

//==============================================================================
void MyArpeggiatorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MyArpeggiatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyArpeggiatorAudioProcessor();
}

// Calculation methods =========================================================
int MyArpeggiatorAudioProcessor::getSamplesPerBeat(AudioPlayHead::CurrentPositionInfo currentPosInfo) {
	double beatsPerSec = currentPosInfo.bpm / 60.0;
	double secPerBeat = 1.0 / beatsPerSec;
	return ((int)(secPerBeat * rate));
}

float MyArpeggiatorAudioProcessor::GetClosestMeter(float meter)
{
	float closestMeter;
	if (meter <= 0.25)
	{
		closestMeter = 1.0 / 32.0;
	}
	else if (meter > 0.25 && meter <= 0.5)
	{
		closestMeter = 1.0 / 16.0;
	}
	else if (meter > 0.5 && meter <= 0.75)
	{
		closestMeter = 1.0 / 8.0; //eight notes = 2 notes per beat
	}
	else
	{
		closestMeter = 1.0 / 4.0; //quarter notes = note per beat
	}

	return closestMeter;
}