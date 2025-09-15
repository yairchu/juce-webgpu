#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class WebGPUGraphics;

class MainComponent : public juce::Component, private juce::Timer
{
public:
    MainComponent();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    std::unique_ptr<WebGPUGraphics> webgpuGraphics;

    juce::Label statusLabel;
    juce::Image renderedImage;

    bool isInitialized = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
