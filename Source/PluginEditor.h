/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "TablesComponent.h"
#include "SurjectiveMidiKeyboardComponent.h"
#include "FilePicker.h"
#include "SlidersComponent.h"

using juce::SurjectiveMidiKeyboardComponent;

//==============================================================================
/**
*/
class JuicySFAudioProcessorEditor
: public AudioProcessorEditor
, public ValueTree::Listener
, private Value::Listener
{
public:
    JuicySFAudioProcessorEditor(
      JuicySFAudioProcessor&,
      AudioProcessorValueTreeState& valueTreeState
      );
    ~JuicySFAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    bool keyPressed(const KeyPress &key) override;
    bool keyStateChanged (bool isKeyDown) override;

    virtual void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged,
        const Identifier& property) override;
    inline virtual void valueTreeChildAdded(ValueTree& parentTree,
        ValueTree& childWhichHasBeenAdded) override {
    };
    inline virtual void valueTreeChildRemoved(ValueTree& parentTree,
        ValueTree& childWhichHasBeenRemoved,
        int indexFromWhichChildWasRemoved) override {
    };
    inline virtual void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved,
        int oldIndex, int newIndex) override {
    };
    inline virtual void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override {};
    inline virtual void valueTreeRedirected(ValueTree& treeWhichHasBeenChanged) override {};

private:
    void valueChanged (Value&) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuicySFAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    Value lastUIWidth, lastUIHeight;

    SurjectiveMidiKeyboardComponent midiKeyboard;
    TablesComponent tablesComponent;
    SlidersComponent slidersComponent;

    FilePicker soundFontFilePicker;
    Label soundFontFilePickerLabel;
    TextButton soundFontFileClearButton;

    FilePicker noteNamesFilePicker;
    Label noteNamesFilePickerLabel;
    TextButton noteNamesFileClearButton;

    bool focusInitialized;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuicySFAudioProcessorEditor)
};
