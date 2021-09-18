#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VkManagement/FrameBuffer.h>
#include <Platform/Vulkan/VkManagement/RenderPass.h>

namespace nuts {

    void FrameBuffer::init(vk::Device device, vk::RenderPass renderPass) noexcept {
        mDevice     = device;
        mRenderPass = renderPass;
    }
    void FrameBuffer::finalize() noexcept {
        if (mFrameBuffers.size() > 0) {
            destroyFrameBuffers();
        }
    }
    void FrameBuffer::createFrameBuffers(const SwapChain::Data& swapChainData) {
        const auto frameBufferSize = swapChainData.swapChainImages->size();
        mFrameBuffers.resize(frameBufferSize);

        for (uint32_t i = 0; i < frameBufferSize; ++i) {
            std::array< vk::ImageView, 1 > attachments = { swapChainData.swapChainImages->at(i).imageView };

            vk::FramebufferCreateInfo frameBufferCreateInfo = {};
            frameBufferCreateInfo.renderPass                = mRenderPass;
            frameBufferCreateInfo.attachmentCount           = static_cast< uint32_t >(attachments.size());
            frameBufferCreateInfo.pAttachments              = attachments.data();
            frameBufferCreateInfo.width                     = swapChainData.extent.width;
            frameBufferCreateInfo.height                    = swapChainData.extent.height;
            frameBufferCreateInfo.layers                    = 1;

            if (vk::Result::eSuccess != mDevice.createFramebuffer(&frameBufferCreateInfo, nullptr, &mFrameBuffers[i])) {
                throw std::runtime_error("Failed to create a Framebuffer!");
            }
        }
    }
    void FrameBuffer::updateFrameBuffers(const SwapChain::Data& swapChainData) {
        destroyFrameBuffers();
        createFrameBuffers(swapChainData);
    }
    void FrameBuffer::destroyFrameBuffers() {
        for (auto& frameBuffer : mFrameBuffers) {
            mDevice.destroyFramebuffer(frameBuffer);
            frameBuffer = nullptr;
        }
        mFrameBuffers.resize(0);
    }

    const Vector< vk::Framebuffer >* const FrameBuffer::getFrameBuffers() const noexcept {
        return &mFrameBuffers;
    }
    FrameBuffer::size_type FrameBuffer::getFrameBufferCount() const noexcept {
        return mFrameBuffers.size();
    }

} // namespace nuts
