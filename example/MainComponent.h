#pragma once

#include "WebGPUComponent.h"
#include "WebGPUExampleScene.h"

class MainComponent : public WebGPUComponent
{
public:
    MainComponent();

    void renderFrame() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    WebGPUExampleScene scene;
    juce::Label statusLabel;
    bool isInitialized = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
