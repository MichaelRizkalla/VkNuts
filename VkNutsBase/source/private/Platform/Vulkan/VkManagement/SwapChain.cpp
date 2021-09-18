#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VkManagement/SwapChain.h>

namespace nuts {

    void SwapChain::init(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surfaceKHR, QueueFamilyIndices queueFamilyIndices,
                         IWindow* window) noexcept {
        mDevice             = device;
        mPhysicalDevice     = physicalDevice;
        mSurfaceKHR         = surfaceKHR;
        mQueueFamilyIndices = queueFamilyIndices;
        mWindow             = window;
        mActiveImageIndex   = std::numeric_limits< decltype(mActiveImageIndex) >::max();
        mActiveSyncIndex    = 0;
    }
    void SwapChain::finalize() noexcept {
        if (mSwapChain) {
            destroySyncObjects();
            destroySwapChain();
        }
    }

    vk::Extent2D SwapChain::getExtent() const noexcept {
        return mSwapChainExtent;
    }
    uint32_t SwapChain::getWidth() const noexcept {
        return mSwapChainExtent.width;
    }
    uint32_t SwapChain::getHeight() const noexcept {
        return mSwapChainExtent.height;
    }
    SwapChain::Details SwapChain::getDetails() const noexcept {
        return mDetails;
    }
    SwapChain::Data SwapChain::getData() const noexcept {
        return { &mSwapChainImages, mSwapChainExtent, mSwapChainImageFormat };
    }
    vk::SwapchainKHR SwapChain::getVkHandle() const noexcept {
        return mSwapChain;
    }
    vk::Format SwapChain::getSwapChainImageFormat() const noexcept {
        return mSwapChainImageFormat;
    }
    vk::SurfaceFormatKHR SwapChain::getSwapChainSurfaceFormatKHR() const noexcept {
        return mSwapChainSurfaceFormatKHR;
    }
    const Vector< Texture >* const SwapChain::getSwapChainImages() const noexcept {
        return &mSwapChainImages;
    }
    SwapChain::size_type SwapChain::getSwapChainImageCount() const noexcept {
        return mSwapChainImages.size();
    }

    uint32_t SwapChain::getActiveImageIndex() const noexcept {
        return mActiveImageIndex;
    }
    uint32_t SwapChain::getActiveSyncObjectIndex() const noexcept {
        return mActiveSyncIndex % mSwapChainImages.size();
    }
    uint32_t SwapChain::acquireNextImage() {
        if (vk::Result::eSuccess != mDevice.waitForFences(mSubmitFences.at(getActiveSyncObjectIndex()), true, std::numeric_limits< uint64_t >::max())) {
            // TODO: Better manage exception
            throw std::exception { "Failed to wait for Fences!" };
        }
        mDevice.resetFences(mSubmitFences.at(getActiveSyncObjectIndex()));
        for (auto i = 0; i < 2; ++i) {
            auto result = mDevice.acquireNextImageKHR(mSwapChain, std::numeric_limits< uint64_t >::max(), mImageAvailableSemaphore.at(getActiveSyncObjectIndex()),
                                                      vk::Fence {}, &mActiveImageIndex);
            if (result == vk::Result::eSuccess) {
                return mActiveImageIndex;
            } else if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
                updateSwapChain(nullptr);
            }
        }
        // TODO: Better manage exception
        throw std::exception { "Failed to acquire next image!" };
    }
    void SwapChain::submit(vk::Queue queue, vk::CommandBuffer commandBuffer, Vector< vk::PipelineStageFlags > waitStages) {
        vk::SubmitInfo submitInfo {};
        submitInfo.waitSemaphoreCount   = 1;
        submitInfo.pWaitSemaphores      = &mImageAvailableSemaphore.at(getActiveSyncObjectIndex());
        submitInfo.pWaitDstStageMask    = waitStages.data();
        submitInfo.commandBufferCount   = 1;
        submitInfo.pCommandBuffers      = &commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = &mWaitSemaphore.at(getActiveSyncObjectIndex());

        queue.submit(submitInfo, mSubmitFences.at(getActiveSyncObjectIndex()));
    }
    void SwapChain::present(vk::Queue queue) {
        vk::PresentInfoKHR presentInfo {};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = &mWaitSemaphore.at(getActiveSyncObjectIndex());
        presentInfo.swapchainCount     = 1;
        presentInfo.pSwapchains        = &mSwapChain;
        presentInfo.pImageIndices      = &mActiveImageIndex;

        if (vk::Result::eSuccess != queue.presentKHR(presentInfo)) {
            // TODO: Better manage exception
            throw std::exception { "Failed to present Swap Chain image!" };
        }

        ++mActiveSyncIndex;
    }

    void SwapChain::createSwapChain(bool update) {
        mActiveImageIndex = std::numeric_limits< decltype(mActiveImageIndex) >::max();
        updateDetails();

        mSwapChainExtent           = ChooseSwapExtent(mDetails.surfaceCapabilitiesKHR);
        mSwapChainSurfaceFormatKHR = ChooseSurfaceFormatKHR(mDetails.surfaceFormatsKHR);
        mSwapChainPresentModeKHR   = ChoosePresentationModeKHR(mDetails.presentationModesKHR);

        uint32_t imageCount = mDetails.surfaceCapabilitiesKHR.minImageCount + 1;

        if (mDetails.surfaceCapabilitiesKHR.maxImageCount > 0 && mDetails.surfaceCapabilitiesKHR.maxImageCount < imageCount) {
            imageCount = mDetails.surfaceCapabilitiesKHR.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR swapChainCreateInfo {};
        swapChainCreateInfo.setSurface(mSurfaceKHR)
            .setMinImageCount(imageCount)
            .setImageFormat(mSwapChainSurfaceFormatKHR.format)
            .setImageColorSpace(mSwapChainSurfaceFormatKHR.colorSpace)
            .setImageExtent(mSwapChainExtent)
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setPreTransform(mDetails.surfaceCapabilitiesKHR.currentTransform)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setPresentMode(mSwapChainPresentModeKHR)
            .setClipped(VK_TRUE);

        uint32_t queueFamilyIndices[2] {};
        if (mQueueFamilyIndices.graphicsFamily.value() != mQueueFamilyIndices.presentFamily.value()) {
            queueFamilyIndices[0] = mQueueFamilyIndices.graphicsFamily.value();
            queueFamilyIndices[1] = mQueueFamilyIndices.presentFamily.value();

            swapChainCreateInfo.setImageSharingMode(vk::SharingMode::eConcurrent).setQueueFamilyIndexCount(2).setPQueueFamilyIndices(queueFamilyIndices);
        } else {
            swapChainCreateInfo.setImageSharingMode(vk::SharingMode::eExclusive).setQueueFamilyIndexCount(1).setPQueueFamilyIndices(nullptr);
        }

        swapChainCreateInfo.setOldSwapchain(update ? mSwapChain : nullptr);

        if (vk::Result::eSuccess != mDevice.createSwapchainKHR(&swapChainCreateInfo, nullptr, &mSwapChain)) {
            throw std::runtime_error("Failed to create SwapChain!");
        }
        mSwapChainImageFormat = mSwapChainSurfaceFormatKHR.format;

        vk::ImageViewCreateInfo imageViewCreateInfo {};
        imageViewCreateInfo.viewType                        = vk::ImageViewType::e2D;
        imageViewCreateInfo.format                          = mSwapChainImageFormat;
        imageViewCreateInfo.components.r                    = vk::ComponentSwizzle::eIdentity;
        imageViewCreateInfo.components.g                    = vk::ComponentSwizzle::eIdentity;
        imageViewCreateInfo.components.b                    = vk::ComponentSwizzle::eIdentity;
        imageViewCreateInfo.components.a                    = vk::ComponentSwizzle::eIdentity;
        imageViewCreateInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
        imageViewCreateInfo.subresourceRange.levelCount     = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount     = 1;

        auto images = mDevice.getSwapchainImagesKHR< NutsAllocator< vk::Image > >(mSwapChain);
        mSwapChainImages.resize(images.size());

        auto index = 0;
        for (auto image : images) {
            mSwapChainImages[index].image = image;

            imageViewCreateInfo.image = image;
            if (vk::Result::eSuccess != mDevice.createImageView(&imageViewCreateInfo, nullptr, &mSwapChainImages[index].imageView)) {
                throw std::runtime_error("Failed to create image view!");
            }
            ++index;
        }

        // Create sync objects
        /*if (!update) {*/
        vk::SemaphoreCreateInfo semaphoreCreateInfo {};
        mImageAvailableSemaphore.resize(getSwapChainImageCount());
        mWaitSemaphore.resize(getSwapChainImageCount());
        for (auto& semaphore : mImageAvailableSemaphore) {
            semaphore = mDevice.createSemaphore(semaphoreCreateInfo);
        }
        for (auto& semaphore : mWaitSemaphore) {
            semaphore = mDevice.createSemaphore(semaphoreCreateInfo);
        }

        vk::FenceCreateInfo fenceCreateInfo {};
        fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled;
        mSubmitFences.resize(getSwapChainImageCount());
        for (auto& fence : mSubmitFences) {
            fence = mDevice.createFence(fenceCreateInfo);
        }
        /*}*/
    }
    void SwapChain::updateSwapChain(vk::Queue queue) {
        if (queue) {
            queue.waitIdle();
        }
        mDevice.waitIdle();

        destroySyncObjects();
        for (auto& texture : mSwapChainImages) {
            mDevice.destroyImageView(texture.imageView);
            texture.imageView = nullptr;
        }
        createSwapChain(true);
    }
    void SwapChain::destroySwapChain() {
        for (auto& texture : mSwapChainImages) {
            mDevice.destroyImageView(texture.imageView);
            texture.imageView = nullptr;
        }
        mDevice.destroySwapchainKHR(mSwapChain);
        mSwapChain = nullptr;
    }
    void SwapChain::destroySyncObjects() {
        for (auto& fence : mSubmitFences) {
            mDevice.destroyFence(fence);
            fence = nullptr;
        }
        for (auto& semaphore : mImageAvailableSemaphore) {
            mDevice.destroySemaphore(semaphore);
            semaphore = nullptr;
        }
        for (auto& semaphore : mWaitSemaphore) {
            mDevice.destroySemaphore(semaphore);
            semaphore = nullptr;
        }
        mSubmitFences.resize(0);
        mImageAvailableSemaphore.resize(0);
        mWaitSemaphore.resize(0);
        mActiveSyncIndex = 0;
    }

    void SwapChain::updateDetails() {
        mDetails.surfaceCapabilitiesKHR = mPhysicalDevice.getSurfaceCapabilitiesKHR(mSurfaceKHR);

        mDetails.surfaceFormatsKHR = mPhysicalDevice.getSurfaceFormatsKHR< NutsAllocator< vk::SurfaceFormatKHR > >(mSurfaceKHR);

        mDetails.presentationModesKHR = mPhysicalDevice.getSurfacePresentModesKHR< NutsAllocator< vk::PresentModeKHR > >(mSurfaceKHR);
    }
    vk::SurfaceFormatKHR SwapChain::ChooseSurfaceFormatKHR(const Vector< vk::SurfaceFormatKHR >& formats) {
        const bool isOneFormatAvailable   = formats.size() == 1;
        const bool isFirstFormatUndefined = formats[0].format == vk::Format::eUndefined;

        if (isOneFormatAvailable && isFirstFormatUndefined)
            return { vk::Format::eR8G8B8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };

        for (const auto& format : formats) {
            bool const isFormatRGBA     = format.format == vk::Format::eR8G8B8A8Unorm;
            bool const isFormatBGRA     = format.format == vk::Format::eB8G8R8A8Unorm;
            bool const isColorSpaceSRGB = format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;

            if ((isFormatRGBA || isFormatBGRA) && isColorSpaceSRGB)
                return format;
        }

        return formats[0];
    }
    vk::PresentModeKHR SwapChain::ChoosePresentationModeKHR(const Vector< vk::PresentModeKHR >& presentationModes) {
        for (const auto& presentationMode : presentationModes) {
            if (presentationMode == vk::PresentModeKHR::eMailbox)
                return presentationMode;
        }

        return vk::PresentModeKHR::eFifo;
    }
    vk::Extent2D SwapChain::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities) {
        const bool isCurrentExtentValid = surfaceCapabilities.currentExtent.width != std::numeric_limits< uint32_t >::max();

        if (isCurrentExtentValid) {
            return surfaceCapabilities.currentExtent;
        } else {
            int width;
            int height;
            glfwGetFramebufferSize(mWindow->getHandle(), &width, &height);

            vk::Extent2D extent {};
            extent.width  = static_cast< uint32_t >(width);
            extent.height = static_cast< uint32_t >(height);

            extent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, extent.width));

            extent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, extent.height));

            return extent;
        }
    }
} // namespace nuts
