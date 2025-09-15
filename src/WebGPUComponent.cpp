#include "WebGPUComponent.h"

#include "WebGPUJuceUtils.h"

WebGPUComponent::WebGPUComponent()
{
    context.init();
    startTimer (16); // ~60 FPS
}

void WebGPUComponent::paint (juce::Graphics& g)
{
    if (! renderedImage.isNull())
        g.drawImage (renderedImage, getLocalBounds().toFloat());
}

void WebGPUComponent::resized()
{
    if (getWidth() == (int) texture.descriptor.size.width && getHeight() == (int) texture.descriptor.size.height)
        return;

    renderedImage = juce::Image (juce::Image::ARGB, getWidth(), getHeight(), true);
    texture.init (context, {
                               .usage = wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc,
                               .dimension = WGPUTextureDimension_2D,
                               .size = { (uint32_t) getWidth(), (uint32_t) getHeight(), 1 },
                               .format = WGPUTextureFormat_BGRA8Unorm,
                               .mipLevelCount = 1,
                               .sampleCount = 1,
                           });
    renderToImage();
}

void WebGPUComponent::timerCallback()
{
    renderToImage();
}

void WebGPUComponent::renderToImage()
{
    renderFrame();
    WebGPUJuceUtils::readTextureToImage (context, texture, renderedImage);
}
