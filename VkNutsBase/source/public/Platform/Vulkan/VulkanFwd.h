#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {

    struct VkSamplerManager;

    class VulkanMemoryAllocator;
    class SwapChain;
    class FrameBuffer;
    class CommandBuffer;
    class RenderPass;

    struct Buffer;
    struct Image;
    struct Texture;
    struct AccelerationStructureKHR;
    struct AllocationChunk;

    struct QueueFamilyIndices {
        std::optional< uint32_t > graphicsFamily;
        std::optional< uint32_t > presentFamily;
        std::optional< uint32_t > computeFamily;

        bool isComplete() const noexcept {
            return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value();
        }
    };

} // namespace nuts
