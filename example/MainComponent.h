#pragma once

#include "WebGPUExampleScene.h"
#include "WebGPUUtils.h"
#include <atomic>
#include <juce_gui_basics/juce_gui_basics.h>

class MainComponent : public juce::Component, private juce::Timer
{
public:
    MainComponent();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    // WebGPU functionality (moved from WebGPUGraphics)
    bool initializeWebGPU (int width, int height);
    void resizeWebGPU (int width, int height);
    juce::Image renderFrameToImage();
    void renderFrame();
    bool createTexture (int width, int height);

    // WebGPU state (moved from WebGPUGraphics)
    std::atomic<bool> webgpuInitialized { false };
    std::atomic<bool> shutdownRequested { false };
    WebGPUContext context;
    WebGPUExampleScene scene;
    WebGPUTexture texture;

    // UI components
    juce::Label statusLabel;
    juce::Image renderedImage;

    bool isInitialized = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
