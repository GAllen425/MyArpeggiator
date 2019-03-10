/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyArpeggiatorAudioProcessorEditor::MyArpeggiatorAudioProcessorEditor (MyArpeggiatorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	StringArray keyStringList = {"None" , "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
	
	addAndMakeVisible(octaveDownKnob = new Slider("Octave Down"));
	octaveDownKnob->setSliderStyle(Slider::Rotary);
	octaveDownKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

	addAndMakeVisible(octaveUpKnob = new Slider("Octave Up"));
	octaveUpKnob->setSliderStyle(Slider::Rotary);
	octaveUpKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
	
	addAndMakeVisible(minNoteLengthKnob = new Slider("Min Note Length"));
	minNoteLengthKnob->setSliderStyle(Slider::Rotary);
	minNoteLengthKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

	addAndMakeVisible(maxNoteLengthKnob = new Slider("Max Note Length"));
	maxNoteLengthKnob->setSliderStyle(Slider::Rotary);
	maxNoteLengthKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

	addAndMakeVisible(meterKnob = new Slider("Meter"));
	meterKnob->setSliderStyle(Slider::Rotary);
	meterKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

	addAndMakeVisible(grooveKnob = new Slider("Groove"));
	grooveKnob->setSliderStyle(Slider::Rotary);
	grooveKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

	addAndMakeVisible(weirdnessKnob = new Slider("Weirdness"));
	weirdnessKnob->setSliderStyle(Slider::Rotary);
	weirdnessKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

	addAndMakeVisible(keyComboBox = new ComboBox("Key"));
	keyComboBox->addItemList(keyStringList, 1);

	meterAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "meter", *meterKnob);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 400);
}

MyArpeggiatorAudioProcessorEditor::~MyArpeggiatorAudioProcessorEditor()
{
}

//==============================================================================
void MyArpeggiatorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

	g.drawText("Octave Down", ((getWidth() / 5) * 1) - (100 / 2), ((getHeight() / 3) * 1) + 5, 100, 100, Justification::centred, false);
	g.drawText("Octave Up", ((getWidth() / 5) * 1) - (100 / 2), ((getHeight() / 3) * 2) + 5, 100, 100, Justification::centred, false);
	g.drawText("Min Note Length", ((getWidth() / 5) * 2) - (100 / 2), ((getHeight() / 3) * 1) + 5, 110, 100, Justification::centred, false);
	g.drawText("Max Note Length", ((getWidth() / 5) * 2) - (100 / 2), ((getHeight() / 3) * 2) + 5, 110, 100, Justification::centred, false);
	g.drawText("Meter", ((getWidth() / 5) * 3) - (100 / 2), ((getHeight() / 3) * 1) + 5, 100, 100, Justification::centred, false);
	g.drawText("Groove", ((getWidth() / 5) * 3) - (100 / 2), ((getHeight() / 3) * 2) + 5, 100, 100, Justification::centred, false);
	g.drawText("Weirdness", ((getWidth() / 5) * 4) - (100 / 2), ((getHeight() / 3) * 1) + 5, 100, 100, Justification::centred, false);

}

void MyArpeggiatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	octaveDownKnob->setBounds(((getWidth() / 5) * 1) - (100 / 2), ((getHeight() / 3) * 1) - (100 / 2), 100, 100);
	octaveUpKnob->setBounds(((getWidth() / 5) * 1) - (100 / 2), ((getHeight() / 3) * 2)  - (100 / 2), 100, 100);
	minNoteLengthKnob->setBounds(((getWidth() / 5) * 2) - (100 / 2), ((getHeight() / 3) * 1)  - (100 / 2), 100, 100);
	maxNoteLengthKnob->setBounds(((getWidth() / 5) * 2) - (100 / 2), ((getHeight() / 3) * 2)  - (100 / 2), 100, 100);
	meterKnob->setBounds(((getWidth() / 5) * 3) - (100 / 2), ((getHeight() / 3) * 1)  - (100 / 2), 100, 100);
	grooveKnob->setBounds(((getWidth() / 5) * 3) - (100 / 2), ((getHeight() / 3) * 2)  - (100 / 2), 100, 100);
	weirdnessKnob->setBounds(((getWidth() / 5) * 4) - (100 / 2), ((getHeight() / 3) * 1)  - (100 / 2), 100, 100);
	keyComboBox->setBounds(((getWidth() / 5) * 4) - 35, ((getHeight() / 3) * 2 + 40)  - (100 / 2), 75, 25);
}
