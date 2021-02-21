#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/Utilities.hpp>
#include <iostream>
#include <memory>
#include <vector>

#define VULKAN_HPP_STORAGE_SHARED
#if defined(NUTS_BUILD_DLL)
    #define VULKAN_HPP_STORAGE_SHARED_EXPORT
#endif

#define VK_USE_PLATFORM_WIN32_KHR
#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

#include <Platform/Vulkan/IWindow.h>

namespace nuts {

    enum class RuntimeValidation { Disable, Enable };

    enum class PhsyicalDeviceSelectionStrategy {
        SelectFirst,
        SelectLast,
        SelectLatestAPI,
    };

    enum class ImageUsage {
        MSAA,
        DepthTest,
        ShadowMap,
    };

    struct InstanceData {
        const char*       name;
        uint32_t          minVersion;
        IWindow*          window;
        RuntimeValidation validation;
    };

    struct QueuePopulate {
        vk::QueueFlags flags;
        vk::SurfaceKHR surface;
        uint32_t       queueFamilyId;
        float          priority;
    };

    struct DeviceData {
        vk::PhysicalDevice           physicalDevice;
        std::vector< QueuePopulate > queues;
        RuntimeValidation            validation;
    };

    struct SwapchainData {
        vk::SurfaceKHR                  surface;
        vk::SurfaceFormatKHR            surfaceFormat;
        vk::Extent2D                    extent;
        vk::PresentModeKHR              presentMode;
        uint32_t                        count;
        vk::SurfaceTransformFlagBitsKHR transform;
    };

    struct ImageData {
        vk::Extent3D            extent;
        vk::Format              format;
        uint32_t                layers;
        vk::SampleCountFlagBits sampleCount;
        ImageUsage              usage;
    };

    struct SamplerData {
        vk::Filter             minFilter;
        vk::Filter             magFilter;
        vk::SamplerMipmapMode  mipmapMode;
        vk::SamplerAddressMode addressMode;
        float                  mipLodBias;
        vk::Bool32             anistropyEnable;
        float                  maxAnistropy;
        vk::Bool32             CompareEnable;
        vk::CompareOp          compareOp;
        float                  minLod;
        float                  maxLod;
        vk::BorderColor        borderColor;
        vk::Bool32             unnormalizedCoords;
    };

    struct VulkanHelper {
        VulkanHelper()                    = delete;
        VulkanHelper(const VulkanHelper&) = delete;
        VulkanHelper(VulkanHelper&&)      = delete;
        VulkanHelper& operator=(const VulkanHelper&) = delete;
        VulkanHelper& operator=(VulkanHelper&&) = delete;
        ~VulkanHelper()                         = delete;

        // Create instance and device
        NUTS_API [[nodiscard]] static vk::Instance CreateInstance(const InstanceData& instanceData, uint32_t requestedExtensionsCount, const char** pRequestedExtensions);
        NUTS_API [[nodiscard]] static vk::Instance CreateInstance(const InstanceData& instanceData);
        NUTS_API [[nodiscard]] static vk::Device   CreateDevice(const DeviceData& deviceData);
        NUTS_API [[nodiscard]] static vk::Device   CreateDevice(const DeviceData& deviceData, uint32_t requestedDeviceExtensionsCount,
                                                                const char** pRequestedDeviceExtensions);
        NUTS_API [[nodiscard]] static vk::SurfaceKHR CreateSurface(const vk::Instance& instance, IWindow* window);

        // Selectors
        NUTS_API [[nodiscard]] static vk::PhysicalDevice SelectPhysicalDevice(const vk::Instance& instance, PhsyicalDeviceSelectionStrategy strategy);
        NUTS_API [[nodiscard]] static uint32_t SelectQueueFamilyIndex(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, vk::QueueFlags queueFlags);
        NUTS_API [[nodiscard]] static vk::SurfaceFormatKHR SelectSurfaceFormat(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
                                                                               vk::Format preferredFormat = vk::Format::eR8G8B8A8Unorm);
        NUTS_API [[nodiscard]] static vk::PresentModeKHR   SelectPresentMode(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
                                                                             vk::PresentModeKHR preferredMode = vk::PresentModeKHR::eImmediate);

        // Create vk objects
        NUTS_API [[nodiscard]] static vk::SwapchainKHR CreateSwapchain(const vk::Device& device, const SwapchainData& swapChainData);
        NUTS_API [[nodiscard]] static vk::Image        Create2DImage(const vk::Device& device, const ImageData& imageData);
        NUTS_API [[nodiscard]] static vk::Sampler      CreateSampler(const vk::Device& device, const SamplerData& samplerData);

        // Debugging
        NUTS_API [[nodiscard]] static vk::DebugUtilsMessengerEXT CreateDebugUtilsMessenger(const vk::Instance& instance);

      private:
        static vk::DynamicLoader mDynamicLoader;
    };

} // namespace nuts