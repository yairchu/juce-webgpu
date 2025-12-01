#define WEBGPU_CPP_IMPLEMENTATION

#include "WebGPUUtils.h"

namespace
{
// TODO: Consider using https://github.com/eliemichel/WebGPU-utils instead
uint32_t getBytesPerPixel (WGPUTextureFormat format)
{
    switch (format)
    {
        case WGPUTextureFormat_RGBA8Unorm:
        case WGPUTextureFormat_RGBA8UnormSrgb:
        case WGPUTextureFormat_BGRA8Unorm:
        case WGPUTextureFormat_BGRA8UnormSrgb:
            return 4;
        case WGPUTextureFormat_RG8Unorm:
            return 2;
        case WGPUTextureFormat_R8Unorm:
            return 1;
        case WGPUTextureFormat_RGBA16Float:
            return 8;
        case WGPUTextureFormat_RGBA32Float:
            return 16;
        default:
            // Default to 4 bytes for common RGBA formats
            return 4;
    }
}
} // namespace

bool WebGPUContext::init()
{
    instance = wgpu::createInstance();
    if (! instance)
        return false;

    device = wgpu::raii::Adapter (instance->requestAdapter ({}))->requestDevice ({});
    if (! device)
        return false;

    queue = device->getQueue();
    return queue;
}

wgpu::raii::ShaderModule WebGPUContext::loadWgslShader (const char* source, const char* name)
{
    const WGPUShaderSourceWGSL wgslSource {
        .chain = { .sType = WGPUSType_ShaderSourceWGSL },
        .code = wgpu::StringView (source),
    };
    const WGPUShaderModuleDescriptor shaderDesc {
        .nextInChain = &wgslSource.chain,
        .label = name == nullptr ? WGPUStringView {} : wgpu::StringView (name),
    };
    return wgpu::raii::ShaderModule (wgpuDeviceCreateShaderModule (*device, &shaderDesc));
}

bool WebGPUTexture::init (WebGPUContext& context, const WGPUTextureDescriptor& desc)
{
    descriptor = desc;
    texture = context.device->createTexture (desc);
    if (! texture)
        return false;
    view = texture->createView();
    return view;
}

wgpu::raii::Buffer WebGPUTexture::read (WebGPUContext& context)
{
    const auto rowSize = (uint32_t) bytesPerRow();
    const uint32_t bufferSize = rowSize * descriptor.size.height;

    wgpu::raii::Buffer readbackBuffer = context.device->createBuffer (WGPUBufferDescriptor {
        .usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::MapRead,
        .size = bufferSize,
        .mappedAtCreation = false,
    });

    {
        wgpu::raii::CommandEncoder encoder = context.device->createCommandEncoder();
        encoder->copyTextureToBuffer (
            WGPUTexelCopyTextureInfo {
                .texture = *texture,
                .mipLevel = 0,
                .origin = { 0, 0, 0 },
                .aspect = WGPUTextureAspect_All,
            },
            WGPUTexelCopyBufferInfo {
                .layout = {
                    .offset = 0,
                    .bytesPerRow = rowSize,
                    .rowsPerImage = descriptor.size.height,
                },
                .buffer = *readbackBuffer,
            },
            WGPUExtent3D {
                .width = descriptor.size.width,
                .height = descriptor.size.height,
                .depthOrArrayLayers = 1,
            });
        context.queue->submit (1, &*wgpu::raii::CommandBuffer (encoder->finish()));
    }

    readbackBuffer->mapAsync (
        WGPUMapMode_Read, 0, bufferSize,
        WGPUBufferMapCallbackInfo {
            .mode = WGPUCallbackMode_WaitAnyOnly,
            .callback = [] (WGPUMapAsyncStatus status, WGPUStringView, void*, void*) {
                assert (status == WGPUMapAsyncStatus_Success);
            },
        }
    );
    context.device->poll (true, nullptr);

    return readbackBuffer;
}

int WebGPUTexture::bytesPerRow() const
{
    const uint32_t unalignedBytesPerRow = descriptor.size.width * getBytesPerPixel (descriptor.format);
    const uint32_t alignment = 256;
    return ((unalignedBytesPerRow + alignment - 1) / alignment) * alignment;
}

const char* WebGPUPassThroughFragmentShader::wgslSource = R"(
    @fragment
    fn fragIdent(@location(0) color: vec4<f32>) -> @location(0) vec4<f32> {
        return color;
    }
)";

const char* WebGPUPassThroughFragmentShader::entryPoint = "fragIdent";
