#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/Utilities.hpp>
#include <Platform/Generic/IWindow.h>
#include <Platform/Vulkan/VulkanFwd.h>

namespace nuts {

    enum class RuntimeValidation
    {
        Disable,
        Enable
    };

    enum class PhsyicalDeviceSelectionStrategy
    {
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
        vk::QueueFlags     flags;
        vk::SurfaceKHR     surface;
        QueueFamilyIndices queueFamilyId;
        float              priority;
    };

    struct DeviceData {
        vk::PhysicalDevice           physicalDevice;
        std::vector< QueuePopulate > queues;
        RuntimeValidation            validation;
    };

    struct VulkanHelper {
        STATIC_CLASS(VulkanHelper)

        // Create instance and device
        NUTS_API [[nodiscard]] static vk::Instance createInstance(const InstanceData& instanceData, uint32_t requestedExtensionsCount, const char** pRequestedExtensions);
        NUTS_API [[nodiscard]] static vk::Instance createInstance(const InstanceData& instanceData);
        NUTS_API [[nodiscard]] static vk::Device   createDevice(const DeviceData& deviceData);
        NUTS_API [[nodiscard]] static vk::Device   createDevice(const DeviceData& deviceData, uint32_t requestedDeviceExtensionsCount,
                                                                const char** pRequestedDeviceExtensions);
        NUTS_API [[nodiscard]] static vk::Queue    getQueue(vk::Device device, uint32_t queueFamilyIndex);
        NUTS_API [[nodiscard]] static vk::SurfaceKHR createSurface(const vk::Instance& instance, IWindow* window);

        // Selectors
        NUTS_API [[nodiscard]] static vk::PhysicalDevice selectPhysicalDevice(const vk::Instance& instance, PhsyicalDeviceSelectionStrategy strategy);
        NUTS_API [[nodiscard]] static QueueFamilyIndices selectQueueFamilyIndex(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
                                                                                vk::QueueFlags queueFlags);

        NUTS_API [[nodiscard]] static vk::Format selectImageFormat(vk::PhysicalDevice physicalDevice, const Vector< vk::Format >& formats, vk::ImageTiling imageTiling,
                                                                   vk::FormatFeatureFlags formatFeatureFlags);

        // Debugging
        NUTS_API [[nodiscard]] static vk::DebugUtilsMessengerEXT createDebugUtilsMessenger(const vk::Instance& instance);

      private:
        static vk::DynamicLoader mDynamicLoader;
    };

} // namespace nuts
