#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanHelper.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>
#include <Platform/Generic/IWindow.h>

namespace nuts {

    class SwapChain {
      public:
        using size_type = std::vector< Texture >::size_type;

        DEFAULT_CLASS(SwapChain)

        struct Details {
            vk::SurfaceCapabilitiesKHR     surfaceCapabilitiesKHR {};
            Vector< vk::SurfaceFormatKHR > surfaceFormatsKHR {};
            Vector< vk::PresentModeKHR >   presentationModesKHR {};
        };

        struct Data {
            const Vector< Texture >* const swapChainImages;
            vk::Extent2D                   extent;
            vk::Format                     imageFormat;
        };

        void init(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surfaceKHR, QueueFamilyIndices queueFamilyIndices, IWindow* window) noexcept;
        void finalize() noexcept;

        vk::Extent2D                   getExtent() const noexcept;
        uint32_t                       getWidth() const noexcept;
        uint32_t                       getHeight() const noexcept;
        Details                        getDetails() const noexcept;
        Data                           getData() const noexcept;
        vk::SwapchainKHR               getVkHandle() const noexcept;
        vk::Format                     getSwapChainImageFormat() const noexcept;
        vk::SurfaceFormatKHR           getSwapChainSurfaceFormatKHR() const noexcept;
        const Vector< Texture >* const getSwapChainImages() const noexcept;
        size_type                      getSwapChainImageCount() const noexcept;

        uint32_t getActiveImageIndex() const noexcept;
        uint32_t getActiveSyncObjectIndex() const noexcept;
        uint32_t acquireNextImage();
        void     submit(vk::Queue queue, vk::CommandBuffer commandBuffer, Vector< vk::PipelineStageFlags > waitStages);
        void     present(vk::Queue queue);

        void createSwapChain(bool update = false);
        void updateSwapChain(vk::Queue queue);
        void destroySwapChain();
        void destroySyncObjects();

      private:
        vk::SwapchainKHR        mSwapChain;
        Vector< Texture >       mSwapChainImages;
        Details                 mDetails;
        vk::Format              mSwapChainImageFormat {};
        vk::PresentModeKHR      mSwapChainPresentModeKHR {};
        vk::SurfaceFormatKHR    mSwapChainSurfaceFormatKHR {};
        vk::Extent2D            mSwapChainExtent {};
        Vector< vk::Semaphore > mImageAvailableSemaphore;
        Vector< vk::Semaphore > mWaitSemaphore;
        Vector< vk::Fence >     mSubmitFences;

        uint32_t mActiveImageIndex;
        uint32_t mActiveSyncIndex;

        vk::Device         mDevice;
        vk::PhysicalDevice mPhysicalDevice;
        vk::SurfaceKHR     mSurfaceKHR;
        QueueFamilyIndices mQueueFamilyIndices;
        IWindow*           mWindow;

        void                 updateDetails();
        vk::SurfaceFormatKHR ChooseSurfaceFormatKHR(const Vector< vk::SurfaceFormatKHR >& formats);
        vk::PresentModeKHR   ChoosePresentationModeKHR(const Vector< vk::PresentModeKHR >& presentationModes);
        vk::Extent2D         ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
    };

} // namespace nuts
