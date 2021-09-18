#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>
#include <Platform/Vulkan/VkManagement/SwapChain.h>
#include <Platform/Generic/IWindow.h>

namespace nuts {

    class FrameBuffer {
      public:
        using size_type = Vector< vk::Framebuffer >::size_type;
        DEFAULT_CLASS(FrameBuffer)

        void init(vk::Device device, vk::RenderPass) noexcept;
        void finalize() noexcept;

        void createFrameBuffers(const SwapChain::Data& swapChainData);
        void updateFrameBuffers(const SwapChain::Data& swapChainData);
        void destroyFrameBuffers();

        const Vector< vk::Framebuffer >* const getFrameBuffers() const noexcept;
        size_type                              getFrameBufferCount() const noexcept;

      private:
        vk::RenderPass            mRenderPass;
        vk::Device                mDevice;
        Vector< vk::Framebuffer > mFrameBuffers;
    };

} // namespace nuts
