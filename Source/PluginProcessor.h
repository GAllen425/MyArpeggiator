/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class MyArpeggiatorAudioProcessor  : public AudioProcessor
{
public:

	int octaveDownInt;
	int octaveUpInt;
	int minNoteLength;
	int maxNoteLength;
	float meter;
    //==============================================================================
    MyArpeggiatorAudioProcessor();
    ~MyArpeggiatorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	
	AudioProcessorValueTreeState& getState();
	MidiKeyboardState& getMidiKeyState();

	// Calculation methods =========================================================
	int getSamplesPerBeat(AudioPlayHead::CurrentPositionInfo currentPosInfo);

	float GetClosestMeter(float meter);


private:
	
	ScopedPointer<AudioProcessorValueTreeState> state;
	ScopedPointer<MidiKeyboardState> midiKeyState;

	int currentNote, lastNoteValue;
	int time;
	float rate;
	SortedSet<int> notes;
	int bpm;
	float beatsPerSec;
	float notesPerBeat;
	float notesPerSec;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyArpeggiatorAudioProcessor)
};
