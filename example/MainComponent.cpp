#include "MainComponent.h"

#include "WebGPUJuceUtils.h"

MainComponent::MainComponent()
{
    statusLabel.setText ("Initializing WebGPU...", juce::dontSendNotification);
    statusLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (statusLabel);

    setSize (800, 600);

    isInitialized = initializeWebGPU (getWidth(), getHeight());
    if (isInitialized)
    {
        statusLabel.setText ("WebGPU initialized successfully!", juce::dontSendNotification);
        // Start timer for continuous rendering
        startTimer (16); // ~60 FPS
    }
    else
    {
        statusLabel.setText ("Failed to initialize WebGPU", juce::dontSendNotification);
    }
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    if (! renderedImage.isNull())
        g.drawImage (renderedImage, getLocalBounds().toFloat());
}

void MainComponent::resized()
{
    statusLabel.setBounds (getLocalBounds().removeFromTop (30));
    if (isInitialized)
        resizeWebGPU (getWidth(), getHeight());
}

void MainComponent::timerCallback()
{
    renderedImage = renderFrameToImage();
}

bool MainComponent::initializeWebGPU (int width, int height)
{
    if (webgpuInitialized)
        return true;

    if (! context.init())
        return false;

    if (! scene.initialize (context))
        return false;

    if (! createTexture (width, height))
        return false;

    webgpuInitialized = true;
    juce::Logger::writeToLog ("WebGPU graphics initialized successfully");
    return true;
}

bool MainComponent::createTexture (int width, int height)
{
    return texture.init (context, {
                                      .usage = wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc,
                                      .dimension = WGPUTextureDimension_2D,
                                      .size = { (uint32_t) width, (uint32_t) height, 1 },
                                      .format = WGPUTextureFormat_BGRA8Unorm,
                                      .mipLevelCount = 1,
                                      .sampleCount = 1,
                                  });
}

void MainComponent::resizeWebGPU (int width, int height)
{
    if (! webgpuInitialized || (width == (int) texture.descriptor.size.width && height == (int) texture.descriptor.size.height))
        return;

    createTexture (width, height);
}

juce::Image MainComponent::renderFrameToImage()
{
    if (! webgpuInitialized)
        return {};

    scene.render (context, texture);

    juce::Image image (juce::Image::ARGB, (int) texture.descriptor.size.width, (int) texture.descriptor.size.height, true);
    WebGPUJuceUtils::readTextureToImage (context, texture, image);

    return image;
}
