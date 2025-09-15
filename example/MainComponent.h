#pragma once

#include "WebGPUComponent.h"
#include "WebGPUExampleScene.h"

class MainComponent : public WebGPUComponent
{
public:
    MainComponent();

    void renderFrame() override;

private:
    WebGPUExampleScene scene;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
