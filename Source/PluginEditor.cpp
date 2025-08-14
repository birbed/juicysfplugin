/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GuiConstants.h"

//==============================================================================
JuicySFAudioProcessorEditor::JuicySFAudioProcessorEditor(
    JuicySFAudioProcessor& p,
    AudioProcessorValueTreeState& valueTreeState)
: AudioProcessorEditor{&p}
, processor{p}
, valueTreeState{valueTreeState}
, midiKeyboard{p.keyboardState, SurjectiveMidiKeyboardComponent::horizontalKeyboard}
, tablesComponent{valueTreeState}
, soundFontFilePicker{valueTreeState, "soundFont", "*.sf2;*.sf3", "Choose a Soundfont file to load into the synthesizer"}
, noteNamesFilePicker{valueTreeState, "noteNamesFile", "*.txt", "Choose an optional text file to change piano roll note names"}
, slidersComponent{valueTreeState, p.getFluidSynthModel()}
{
    // set resize limits for this plug-in
    setResizeLimits(
        GuiConstants::minWidth,
        GuiConstants::minHeight,
        GuiConstants::maxWidth,
        GuiConstants::maxHeight);

    lastUIWidth.referTo(valueTreeState.state.getChildWithName("uiState").getPropertyAsValue("width",  nullptr));
    lastUIHeight.referTo(valueTreeState.state.getChildWithName("uiState").getPropertyAsValue("height", nullptr));

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());

    lastUIWidth.addListener(this);
    lastUIHeight.addListener(this);

    midiKeyboard.setName ("MIDI Keyboard");

    midiKeyboard.setWantsKeyboardFocus(false);
    tablesComponent.setWantsKeyboardFocus(false);

    setWantsKeyboardFocus(true);
    addAndMakeVisible(midiKeyboard);

    addAndMakeVisible(slidersComponent);
    addAndMakeVisible(tablesComponent);

    addAndMakeVisible(soundFontFilePicker);
    addAndMakeVisible(soundFontFilePickerLabel);
    soundFontFilePickerLabel.setText("Soundfont: ", dontSendNotification);
    addAndMakeVisible(soundFontFileClearButton);
    soundFontFileClearButton.setButtonText("X");
    soundFontFileClearButton.onClick = [&] {
        Value value{ valueTreeState.state.getChildWithName("soundFont").getPropertyAsValue("path", nullptr)};
        value.setValue("");
    };

    addAndMakeVisible(noteNamesFilePicker);
    addAndMakeVisible(noteNamesFilePickerLabel);
    noteNamesFilePickerLabel.setText("Note Names: ", dontSendNotification);
    addAndMakeVisible(noteNamesFileClearButton);
    noteNamesFileClearButton.setButtonText("X");
    noteNamesFileClearButton.onClick = [&] {
        Value value{ valueTreeState.state.getChildWithName("noteNamesFile").getPropertyAsValue("path", nullptr) };
        value.setValue("");
    };


}

// called when the stored window size changes
void JuicySFAudioProcessorEditor::valueChanged(Value&) {
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());
}

void JuicySFAudioProcessorEditor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged,
    const Identifier& property)
{
    if (treeWhosePropertyHasChanged.getType() == StringRef("noteNamesFile")) {
        if (property == StringRef("path")) {
            String soundFontPath = treeWhosePropertyHasChanged.getProperty("path", "");
            File file = File(soundFontPath);

            juce::StringArray lines;
            lines.addLines(file.loadFileAsString());

            for (int i = 0; i < 132; ++i)
            {
                String propertyName = juce::String(lines.size()-1 - i);
                Value value{ valueTreeState.state.getChildWithName("noteNames").getPropertyAsValue(propertyName, nullptr) };

                if (i < lines.size() && lines[i].isNotEmpty())
                    value.setValue(lines[i]);
                else
                    value.setValue("");
            }

            processor.updateHostDisplay();
        }
    }
}


JuicySFAudioProcessorEditor::~JuicySFAudioProcessorEditor()
{
    lastUIWidth.removeListener(this);
    lastUIHeight.removeListener(this);
}

//==============================================================================
void JuicySFAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    if (!focusInitialized) {
        if (!hasKeyboardFocus(false) && isVisible()) {
            grabKeyboardFocus();
        }
        if (getCurrentlyFocusedComponent() == this) {
            focusInitialized = true;
        }
    }
}

void JuicySFAudioProcessorEditor::resized()
{
    const int padding{8};
    const int pianoHeight{70};
    const int filePickerHeight{25};
    const int soundFontFilePickerLabelWidth{90};
    const int noteNamesFilePickerLabelWidth{100};
    Rectangle<int> r{getLocalBounds()};

    Rectangle<int> topBar = r.removeFromTop(filePickerHeight + padding).reduced(padding, 0).withTrimmedTop(padding);
    soundFontFilePickerLabel.setBounds(topBar.removeFromLeft(soundFontFilePickerLabelWidth));
    soundFontFileClearButton.setBounds(topBar.removeFromRight(filePickerHeight));
    topBar.removeFromRight(padding);
    soundFontFilePicker.setBounds(topBar);

    topBar = r.removeFromTop(filePickerHeight + padding).reduced(padding, 0).withTrimmedTop(padding);
    noteNamesFilePickerLabel.setBounds(topBar.removeFromLeft(noteNamesFilePickerLabelWidth));
    noteNamesFileClearButton.setBounds(topBar.removeFromRight(filePickerHeight));
    topBar.removeFromRight(padding);
    noteNamesFilePicker.setBounds(topBar);

    midiKeyboard.setBounds (r.removeFromBottom (pianoHeight).reduced(padding, 0));

    Rectangle<int> rContent{r.reduced(0, padding)};
    slidersComponent.setBounds(rContent.removeFromRight(slidersComponent.getDesiredWidth() + padding).withTrimmedRight(padding));

    tablesComponent.setBounds(rContent);

    lastUIWidth = getWidth();
    lastUIHeight = getHeight();
}

bool JuicySFAudioProcessorEditor::keyPressed(const KeyPress &key) {
    const int cursorKeys[] = {
            KeyPress::leftKey,
            KeyPress::rightKey,
            KeyPress::upKey,
            KeyPress::downKey
    };
    if (any_of(
            begin(cursorKeys),
            end(cursorKeys),
            [&](int i) { return i == key.getKeyCode(); }
    )) {
        return tablesComponent.keyPressed(key);
    } else {
        return midiKeyboard.keyPressed(key);
    }
    return false;
}

bool JuicySFAudioProcessorEditor::keyStateChanged (bool isKeyDown) {
    return midiKeyboard.keyStateChanged(isKeyDown);
}
