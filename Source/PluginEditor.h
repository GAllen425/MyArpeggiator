/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MyArpeggiatorAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    MyArpeggiatorAudioProcessorEditor (MyArpeggiatorAudioProcessor&);
    ~MyArpeggiatorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyArpeggiatorAudioProcessor& processor;

	ScopedPointer<Slider> octaveDownKnob;
	ScopedPointer<Slider> octaveUpKnob;
	ScopedPointer<Slider> minNoteLengthKnob;
	ScopedPointer<Slider> maxNoteLengthKnob;
	ScopedPointer<Slider> meterKnob;
	ScopedPointer<Slider> grooveKnob;
	ScopedPointer<Slider> weirdnessKnob;
	ScopedPointer<ComboBox> keyComboBox;

	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> octaveDownAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> octaveUpAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> minNoteLengthAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> maxNoteLengthAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> meterAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> grooveAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> weirdnessAttachment;
	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> keyAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyArpeggiatorAudioProcessorEditor)
};
