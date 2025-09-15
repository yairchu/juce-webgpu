#pragma once

#include "WebGPUExampleScene.h"
#include "WebGPUUtils.h"
#include <juce_gui_basics/juce_gui_basics.h>

class MainComponent : public juce::Component, private juce::Timer
{
public:
    MainComponent();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    bool initializeWebGPU (int width, int height);
    void resizeWebGPU (int width, int height);
    juce::Image renderFrameToImage();
    bool createTexture (int width, int height);

    bool webgpuInitialized = false;
    WebGPUContext context;
    WebGPUExampleScene scene;
    WebGPUTexture texture;

    juce::Label statusLabel;
    juce::Image renderedImage;

    bool isInitialized = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
