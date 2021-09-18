#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VkManagement/CommandBuffer.h>

namespace nuts {

    void CommandBuffer::init(vk::Device device, vk::CommandPool commandPool) noexcept {
        mDevice      = device;
        mCommandPool = commandPool;
    }
    void CommandBuffer::finalize() noexcept {
        if (mCommandBuffer) {
            destroyCommandBuffer();
        }
    }
    void CommandBuffer::createCommandBuffer() {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo {};
        commandBufferAllocateInfo.commandPool        = mCommandPool;
        commandBufferAllocateInfo.level              = vk::CommandBufferLevel::ePrimary;
        commandBufferAllocateInfo.commandBufferCount = static_cast< uint32_t >(1 /*mCommandBuffers.size()*/);

        if (vk::Result::eSuccess != mDevice.allocateCommandBuffers(&commandBufferAllocateInfo, &mCommandBuffer)) {
            // TODO: Better manage exceptions
            throw std::runtime_error("Failed to allocate Command Buffers!");
        }
    }
    void CommandBuffer::destroyCommandBuffer() {
        mDevice.freeCommandBuffers(mCommandPool, mCommandBuffer);
    }

    vk::CommandBuffer CommandBuffer::getVkHandle() const noexcept {
        return mCommandBuffer;
    }

} // namespace nuts
