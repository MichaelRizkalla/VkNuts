#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanHelper.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>

namespace nuts {

    class RenderPass {
      public:
        DEFAULT_CLASS(RenderPass)

        void init(vk::Device device) noexcept;
        void finalize() noexcept;

        void createRenderPass(const Vector< vk::Format >& colorAttachmentFormats, vk::Format depthAttachmentFormat, uint32_t renderSubpassCount,
                              vk::ImageLayout initialImageLayout, vk::ImageLayout finalImageLayout, bool doClearColor, bool doClearDepth);

        void destroyRenderPass() noexcept;

        vk::RenderPass getVkHandle() const noexcept;

      private:
        vk::Device     mDevice;
        vk::RenderPass mRenderPass;
    };

} // namespace nuts
