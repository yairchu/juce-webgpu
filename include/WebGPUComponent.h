#pragma once

#include "WebGPUUtils.h"

#include <juce_gui_basics/juce_gui_basics.h>

class WebGPUComponent : public juce::Component, private juce::Timer
{
public:
    WebGPUComponent();

    virtual void renderFrame() = 0;

    void paint (juce::Graphics&) override;
    void resized() override;

    WebGPUContext context;
    WebGPUTexture texture;

private:
    void timerCallback() override;
    void renderToImage();

    juce::Image renderedImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebGPUComponent)
};
