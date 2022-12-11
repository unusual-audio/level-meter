/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LevelMeterAudioProcessorEditor::LevelMeterAudioProcessorEditor (LevelMeterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 270);
    
    startTimer(1000 / 60);
}

LevelMeterAudioProcessorEditor::~LevelMeterAudioProcessorEditor()
{
}

//==============================================================================
void LevelMeterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    g.fillAll (black);
    g.setColour(white);
    g.setFont(18);
    g.drawFittedText("Level Meter", 60, 60, 400, 30, juce::Justification::left, 1);
    
    float level = audioProcessor.getLevel();
    
    g.setColour (grey);
    
    g.drawFittedText (level == audioProcessor.getMinLevel() ? juce::String::fromUTF8(u8"-\u221E dB") : juce::String::formatted("%.1f dB", level), 60, 120, 120, 30, juce::Justification::right, 1);

    int w = 540;
    float maxValue = 0.f;
    float minValue = -80.f;
    g.drawRect(200, 120, w, 30);
    
    g.setFont(10);
    for (auto i : {-60, -50, -40, -35, -30, -25, -20, -15, -10, -5, 0}) {
        float amplitude = (i - minValue) / (maxValue - minValue);
        int x = 200 + static_cast<int>(std::ceil(amplitude * w));
        g.drawLine(x - 0.5, 110, x - 0.5, 120);
        x -= 3;
        g.drawFittedText(juce::String::formatted("%+d", i), x - 10, 90, 20, 20, juce::Justification::centred, 1);
    }
    g.setFont(18);
    
    float amplitude = (level - minValue) / (maxValue - minValue);
    amplitude = amplitude < 0 ? 0 : amplitude > 1 ? 1 : amplitude;
    
    g.setColour (yellow);
    if (level >= -9.f)
        g.setColour (red);
    g.fillRect(200, 120, static_cast<int>(std::ceil(amplitude * w)), 30);

    g.setColour (yellow);
    g.drawFittedText("Unusual Audio", 60, getHeight() - 77, 400, 30, juce::Justification::left, 1);
}

void LevelMeterAudioProcessorEditor::timerCallback()
{
    repaint();
}

void LevelMeterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
