#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/Utilities.hpp>
#include <Platform/Vulkan/IWindow.h>

namespace nuts {

    enum class RuntimeValidation { Disable, Enable };

    enum class PhsyicalDeviceSelectionStrategy {
        SelectFirst,
        SelectLast,
        SelectLatestAPI,
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

    struct VulkanHelper {
        STATIC_CLASS(VulkanHelper)

        // Create instance and device
        NUTS_API [[nodiscard]] static vk::Instance createInstance(const InstanceData& instanceData, uint32_t requestedExtensionsCount, const char** pRequestedExtensions);
        NUTS_API [[nodiscard]] static vk::Instance createInstance(const InstanceData& instanceData);
        NUTS_API [[nodiscard]] static vk::Device   createDevice(const DeviceData& deviceData);
        NUTS_API [[nodiscard]] static vk::Device   createDevice(const DeviceData& deviceData, uint32_t requestedDeviceExtensionsCount,
                                                                const char** pRequestedDeviceExtensions);
        NUTS_API [[nodiscard]] static vk::SurfaceKHR createSurface(const vk::Instance& instance, IWindow* window);

        // Selectors
        NUTS_API [[nodiscard]] static vk::PhysicalDevice selectPhysicalDevice(const vk::Instance& instance, PhsyicalDeviceSelectionStrategy strategy);
        NUTS_API [[nodiscard]] static uint32_t selectQueueFamilyIndex(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, vk::QueueFlags queueFlags);
        NUTS_API [[nodiscard]] static vk::SurfaceFormatKHR selectSurfaceFormat(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
                                                                               vk::Format preferredFormat = vk::Format::eR8G8B8A8Unorm);
        NUTS_API [[nodiscard]] static vk::PresentModeKHR   selectPresentMode(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
                                                                             vk::PresentModeKHR preferredMode = vk::PresentModeKHR::eImmediate);

        // Create vk swapchain
        NUTS_API [[nodiscard]] static vk::SwapchainKHR createSwapchain(const vk::Device& device, const SwapchainData& swapChainData);

        // Debugging
        NUTS_API [[nodiscard]] static vk::DebugUtilsMessengerEXT createDebugUtilsMessenger(const vk::Instance& instance);

      private:
        static vk::DynamicLoader mDynamicLoader;
    };

} // namespace nuts