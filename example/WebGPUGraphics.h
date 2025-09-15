#pragma once

#include <atomic>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include <mutex>
#include <webgpu/webgpu-raii.hpp>

#include "WebGPUExampleScene.h"
#include "WebGPUUtils.h"

class WebGPUGraphics
{
public:
    bool initialize (int width, int height);
    void resize (int width, int height);

    juce::Image renderFrameToImage();

    bool isInitialized() const { return initialized; }

private:
    void renderFrame();
    bool createTexture (int width, int height);

    std::atomic<bool> initialized { false };
    std::atomic<bool> shutdownRequested { false };

    WebGPUContext context;
    WebGPUExampleScene scene;
    WebGPUTexture texture;

    static constexpr uint32_t bytesPerPixel = 4; // RGBA8
};
