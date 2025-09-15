#include "WebGPUJuceUtils.h"

#include "WebGPUUtils.h"
#include <juce_graphics/juce_graphics.h>

void WebGPUJuceUtils::readTextureToImage (WebGPUContext& context, WebGPUTexture& texture, juce::Image& image)
{
    jassert (texture.descriptor.size.width == (uint32_t) image.getWidth());
    jassert (texture.descriptor.size.height == (uint32_t) image.getHeight());

    wgpu::raii::Buffer readbackBuffer = texture.read (context);

    const int bytesPerRow = texture.bytesPerRow();
    const auto src = (uint8_t*) readbackBuffer->getConstMappedRange (0, bytesPerRow * texture.descriptor.size.height);
    juce::Image::BitmapData bitmap (image, juce::Image::BitmapData::writeOnly);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

    switch (texture.descriptor.format)
    {
        case WGPUTextureFormat_BGRA8Unorm:
        case WGPUTextureFormat_BGRA8UnormSrgb:
            for (int y = 0; y < (int) texture.descriptor.size.height; ++y)
                std::memcpy (bitmap.getLinePointer (y), src + y * bytesPerRow, (size_t) texture.descriptor.size.width * 4);
            break;
        case WGPUTextureFormat_RGBA8Unorm:
        case WGPUTextureFormat_RGBA8UnormSrgb:
            for (int y = 0; y < (int) texture.descriptor.size.height; ++y)
                for (int x = 0; x < (int) texture.descriptor.size.width; ++x)
                {
                    const int srcIndex = y * bytesPerRow + x * 4;
                    bitmap.setPixelColour (x, y, juce::Colour::fromRGBA (src[srcIndex + 0], src[srcIndex + 1], src[srcIndex + 2], src[srcIndex + 3]));
                }
            break;
        default:
            jassertfalse; // Unsupported format
            break;
    }

#pragma GCC diagnostic pop

    readbackBuffer->unmap();
}
