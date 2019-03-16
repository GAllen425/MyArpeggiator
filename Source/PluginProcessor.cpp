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
	lastPosition = 0;
	outputMidiChannel = 1;
	inputMidiChannel = 0;

	state = new AudioProcessorValueTreeState(*this, nullptr);
	state->createAndAddParameter("meter", "Meter", "Meter", NormalisableRange<float>(1.0, 4.0, 1.0), 1.0, nullptr, nullptr);
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
	// Get Params
	meter = convertGuiMeterToTimebase(*state->getRawParameterValue("meter")); 
	DBG (String(meter));
	// the audio buffer in a midi effect will have zero channels!
	jassert(buffer.getNumChannels() == 0);

	// Get number of samples from buffer (sometimes breaks and sets to 0)
	auto numSamples = buffer.getNumSamples();

	// Process Midi Input
	processInputMidi(midiMessages);

	AudioPlayHead::CurrentPositionInfo playheadPositionInfo;
	getPlayHead()->getCurrentPosition(playheadPositionInfo);

	this->timeSignatureNumerator = playheadPositionInfo.timeSigNumerator;
	this->timeSignatureDenominator = playheadPositionInfo.timeSigDenominator;

	if (playheadPositionInfo.isPlaying)
	{
		auto pulseLength = 60.0 / (playheadPositionInfo.bpm * meter);
		auto pulseSamples = getSampleRate() * pulseLength;

		auto lastPosition = static_cast<int64>(std::floor(playheadPositionInfo.ppqPosition * meter));
		auto position = lastPosition + static_cast<int64>(std::ceil(numSamples / pulseSamples));

		//time in units of notes e.g. quarter notes or eight notes etc
		auto time = calculateNextTime(position);

		if (time < position)
		{
			auto offset = jmax(0, jmin((int)(pulseLength - time), numSamples - 1));

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
	}

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

void MyArpeggiatorAudioProcessor::processInputMidi(MidiBuffer &inMidi) {
	int sample;
	MidiBuffer outMidi;
	MidiMessage message;

	for (MidiBuffer::Iterator i(inMidi); i.getNextEvent(message, sample);) 
	{
		if (inputMidiChannel == 0 || message.getChannel() == inputMidiChannel) 
		{
			if (message.isNoteOn()) 
			{
				notes.add(message.getNoteNumber());
			}
			else if (message.isNoteOff()) 
			{
				notes.removeValue(message.getNoteNumber());
			}
			else 
			{
				outMidi.addEvent(message, sample);
			}
		}
		else 
		{
			outMidi.addEvent(message, sample);
		}
	}

	inMidi.swapWith(outMidi);
}

int MyArpeggiatorAudioProcessor::convertGuiMeterToTimebase(float meter)
{				
	return std::pow(2,(int)meter-1);
}

int64 MyArpeggiatorAudioProcessor::calculateNextTime(int64 position)
{
	int64 result;
	
	return  result = position + meter;
}