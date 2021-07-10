// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanHelper.h>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace nuts {
    vk::DynamicLoader VulkanHelper::mDynamicLoader {};

    vk::Instance VulkanHelper::createInstance(const InstanceData& instanceData) { return createInstance(instanceData, 0, nullptr); }

    vk::Instance VulkanHelper::createInstance(const InstanceData& instanceData, uint32_t requestedExtensionsCount, const char** pRequestedExtensions) {
        std::pmr::polymorphic_allocator< std::byte > alloc {};

        const size_t nameBufferLen = std::strlen(instanceData.name) + 1;
        char*        nameBuffer    = reinterpret_cast< char* >(alloc.allocate(nameBufferLen));

        std::memcpy(nameBuffer, instanceData.name, nameBufferLen);

        // Dispatch vkInstance creation function
        auto vkGetInstanceProcAddr = mDynamicLoader.getProcAddress< PFN_vkGetInstanceProcAddr >("vkGetInstanceProcAddr");
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        // API Version
        uint32_t apiVersion;
        // TODO: Manage exceptions
        if (vk::Result::eSuccess != vk::enumerateInstanceVersion(&apiVersion)) throw std::exception {};

        uint32_t major = VK_VERSION_MAJOR(apiVersion);
        uint32_t minor = VK_VERSION_MINOR(apiVersion);
        uint32_t patch = VK_VERSION_PATCH(apiVersion);
        // TODO: Manage exceptions
        if (VK_MAKE_VERSION(major, minor, patch) < instanceData.minVersion) throw std::exception {};

#ifdef VKG_LOG

#endif

        auto appInfo = vk::ApplicationInfo { nameBuffer, apiVersion, "vkg", 1, VK_API_VERSION_1_2 };

        auto instanceCreateInfo = vk::InstanceCreateInfo { {}, &appInfo, 0, nullptr, 0, nullptr };

        uint32_t     count;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

        const char*  validation_layers[] = { "VK_LAYER_KHRONOS_validation" };
        const char** extensions;
        if (RuntimeValidation::Enable == instanceData.validation) {
            count += 1;
            extensions = reinterpret_cast< const char** >(alloc.allocate((static_cast< size_t >(count) + requestedExtensionsCount) * sizeof(const char*)));
            std::memcpy(extensions, glfwExtensions, (count - 1) * sizeof(const char*));
            extensions[count - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
            std::memcpy(extensions + (count), pRequestedExtensions, (requestedExtensionsCount) * sizeof(const char*));
        } else {
            extensions = reinterpret_cast< const char** >(alloc.allocate((static_cast< size_t >(count) + requestedExtensionsCount) * sizeof(const char*)));
            std::memcpy(extensions, glfwExtensions, (count) * sizeof(const char*));
            std::memcpy(extensions + (count), pRequestedExtensions, (requestedExtensionsCount) * sizeof(const char*));
        }

        instanceCreateInfo.enabledLayerCount       = static_cast< uint32_t >(array_size::get(validation_layers));
        instanceCreateInfo.ppEnabledLayerNames     = validation_layers;
        instanceCreateInfo.enabledExtensionCount   = count + requestedExtensionsCount;
        instanceCreateInfo.ppEnabledExtensionNames = extensions;

        vk::Instance instance;
        // TODO: Better manage exception
        if (vk::Result::eSuccess != vk::createInstance(&instanceCreateInfo, nullptr, &instance)) throw std::exception {};
        assert(instance);

        // Dispatch VkInstance functions
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

        alloc.deallocate(reinterpret_cast< std::byte* >(extensions), (static_cast< size_t >(count) + requestedExtensionsCount) * sizeof(const char*));
        alloc.deallocate(reinterpret_cast< std::byte* >(nameBuffer), nameBufferLen);

        return instance;
    }

    vk::Device VulkanHelper::createDevice(const DeviceData& deviceData) {
        const char* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        return createDevice(deviceData, static_cast< uint32_t >(array_size::get(device_extensions)), device_extensions);
    }

    vk::Device VulkanHelper::createDevice(const DeviceData& deviceData, uint32_t requestedDeviceExtensionsCount, const char** pRequestedDeviceExtensions) {
        std::pmr::polymorphic_allocator< std::byte > alloc {};

        // Construct Family ID and its priorties
        std::unordered_map< uint32_t, std::vector< float > > queueFamilyIdAndPriorities;
        for (const auto& queue : deviceData.queues) {
            if (queueFamilyIdAndPriorities.find(queue.queueFamilyId) == queueFamilyIdAndPriorities.end()) { queueFamilyIdAndPriorities[queue.queueFamilyId] = {}; }
            queueFamilyIdAndPriorities[queue.queueFamilyId].emplace_back(queue.priority);
        }

        // DeviceQueueCreateInfo struct for each queue
        std::vector< vk::DeviceQueueCreateInfo > queueCreateInfos;

        for (const auto& [id, priorties] : queueFamilyIdAndPriorities) {
            queueCreateInfos.emplace_back(vk::DeviceQueueCreateInfo {});
            auto& queueCreateInfo            = queueCreateInfos.back();
            queueCreateInfo.queueFamilyIndex = id;
            queueCreateInfo.setQueuePriorities(priorties);
        }

        // Device creation stage
        const char* device_layers[] = { "VK_LAYER_KHRONOS_validation" };

        auto     device_features         = deviceData.physicalDevice.getFeatures();
        uint32_t device_extensions_count = requestedDeviceExtensionsCount;

        vk::DeviceCreateInfo deviceCreateInfo {};
        deviceCreateInfo.setQueueCreateInfos(queueCreateInfos)
            .setPEnabledFeatures(&device_features)
            .setEnabledExtensionCount(device_extensions_count)
            .setPpEnabledExtensionNames(pRequestedDeviceExtensions);

        if (RuntimeValidation::Enable == deviceData.validation) {
            deviceCreateInfo.setEnabledLayerCount(static_cast< uint32_t >(array_size::get(device_layers))).setPpEnabledLayerNames(device_layers);
        }

        vk::Device device;
        // TODO: Better manage exception
        if (vk::Result::eSuccess != deviceData.physicalDevice.createDevice(&deviceCreateInfo, nullptr, &device)) throw std::exception {};

        // Dispatch VkDevice functions for the created device
        VULKAN_HPP_DEFAULT_DISPATCHER.init(device);

        return device;
    }

    vk::SurfaceKHR VulkanHelper::createSurface(const vk::Instance& instance, IWindow* window) {
        vk::SurfaceKHR surface {};
        glfwCreateWindowSurface(instance, window->getHandle(), nullptr, reinterpret_cast< VkSurfaceKHR* >(&surface));
        return surface;
    }

    vk::PhysicalDevice VulkanHelper::selectPhysicalDevice(const vk::Instance& instance, PhsyicalDeviceSelectionStrategy strategy) {
        std::pmr::polymorphic_allocator< vk::PhysicalDevice > alloc {};

        uint32_t count = 0;
        // TODO: manage exceptions
        if (vk::Result::eSuccess != instance.enumeratePhysicalDevices(&count, nullptr)) throw std::exception {};
        auto handles = alloc.allocate(count);
        // TODO: Manage exceptions
        if (vk::Result::eSuccess != instance.enumeratePhysicalDevices(&count, handles)) throw std::exception {};

        vk::PhysicalDevice result {};
        switch (strategy) {
            case PhsyicalDeviceSelectionStrategy::SelectFirst:
                result = handles[0];
                break;
            case PhsyicalDeviceSelectionStrategy::SelectLast:
                result = handles[count - 1];
                break;
            case PhsyicalDeviceSelectionStrategy::SelectLatestAPI: {
                uint32_t api_version = 0;
                int32_t  handle_idx  = -1;
                for (uint32_t i = 0; i < count; i++) {
                    auto new_api_version = handles[i].getProperties().apiVersion;
                    if (api_version < new_api_version) {
                        api_version = new_api_version;
                        handle_idx  = i;
                    }
                }
                result = handles[handle_idx];
            } break;
        }
        result = handles[0];

        alloc.deallocate(handles, count);
        return result;
    }

    uint32_t VulkanHelper::selectQueueFamilyIndex(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, vk::QueueFlags queueFlags) {
        std::pmr::polymorphic_allocator< vk::QueueFamilyProperties > alloc {};
        uint32_t                                                     count = 0;
        physicalDevice.getQueueFamilyProperties(&count, nullptr);
        auto properties = alloc.allocate(count);
        physicalDevice.getQueueFamilyProperties(&count, properties);

        auto findQueue = [&](uint32_t idx, const vk::QueueFamilyProperties& properties) {
            vk::Bool32 has_present_support = 0;
            // TODO: Better manage exception
            if (vk::Result::eSuccess != physicalDevice.getSurfaceSupportKHR(idx, surface, &has_present_support)) throw std::exception {};
            return (has_present_support && (properties.queueFlags & queueFlags));
        };

        uint32_t result = std::numeric_limits< uint32_t >::max();
        for (uint32_t i = 0; i < count; ++i) {
            if (findQueue(i, properties[i])) {
                result = i;
                break;
            }
        }
        assert(result != std::numeric_limits< uint32_t >::max());

        alloc.deallocate(properties, count);
        return result;
    }

    vk::SurfaceFormatKHR VulkanHelper::selectSurfaceFormat(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, vk::Format preferredFormat) {
        std::pmr::polymorphic_allocator< vk::SurfaceFormatKHR > alloc {};

        uint32_t count = 0;
        // TODO: Better manage exception
        if (vk::Result::eSuccess != physicalDevice.getSurfaceFormatsKHR(surface, &count, nullptr)) throw std::exception {};
        auto formats = alloc.allocate(count);
        // TODO: Better manage exception
        if (vk::Result::eSuccess != physicalDevice.getSurfaceFormatsKHR(surface, &count, formats)) throw std::exception {};

        // Search for preferredFormat
        vk::SurfaceFormatKHR result {};
        auto                 pResult = std::find_if(formats, formats + count, [&](vk::SurfaceFormatKHR _format) {
            if (_format.format == preferredFormat) return true;
            return false;
        });

        if (pResult != formats + count) {
            result = *pResult;
        } else {
            // TODO: maybe make a pick first policy and output to std::cerr
            throw std::exception {};
        }

        alloc.deallocate(formats, count);
        return result;
    }

    vk::PresentModeKHR VulkanHelper::selectPresentMode(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, vk::PresentModeKHR preferredMode) {
        std::pmr::polymorphic_allocator< vk::PresentModeKHR > alloc {};

        uint32_t count = 0;
        // TODO: Better manage exception
        if (vk::Result::eSuccess != physicalDevice.getSurfacePresentModesKHR(surface, &count, nullptr)) throw std::exception {};
        auto presentModes = alloc.allocate(count);
        // TODO: Better manage exception
        if (vk::Result::eSuccess != physicalDevice.getSurfacePresentModesKHR(surface, &count, presentModes)) throw std::exception {};

        vk::PresentModeKHR result {};

        auto pResult = std::find_if(presentModes, presentModes + count, [&](vk::PresentModeKHR _mode) {
            if (_mode == preferredMode) return true;
            return false;
        });

        if (pResult != presentModes + count) {
            result = *pResult;
        } else {
            // TODO: maybe make a pick first policy and output to std::cerr
            throw std::exception {};
        }

        alloc.deallocate(presentModes, count);
        return result;
    }

    vk::SwapchainKHR VulkanHelper::createSwapchain(const vk::Device& device, const SwapchainData& swapChainData) {
        vk::SwapchainCreateInfoKHR swapChainCreateInfo {};
        swapChainCreateInfo.setSurface(swapChainData.surface)
            .setMinImageCount(swapChainData.count)
            .setImageFormat(swapChainData.surfaceFormat.format)
            .setImageColorSpace(swapChainData.surfaceFormat.colorSpace)
            .setImageExtent(swapChainData.extent)
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setImageSharingMode(vk::SharingMode::eExclusive)
            .setPreTransform(swapChainData.transform)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setPresentMode(swapChainData.presentMode)
            .setClipped(true);

        vk::SwapchainKHR swapchain {};

        // TODO: better manage exception
        if (vk::Result::eSuccess != device.createSwapchainKHR(&swapChainCreateInfo, nullptr, &swapchain)) throw std::exception {};
        return swapchain;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        (void)messageType;
        (void)pUserData;
        std::cerr << "[" << messageSeverity << "]: " << pCallbackData->pMessage << '\n';
        return VK_FALSE;
    }

    vk::DebugUtilsMessengerEXT VulkanHelper::createDebugUtilsMessenger(const vk::Instance& instance) {
        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo {};
        debugUtilsMessengerCreateInfo.setPfnUserCallback(debugCallback);

        auto messageSeverity =
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
        debugUtilsMessengerCreateInfo.setMessageSeverity(messageSeverity);

        auto messageType =
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
        debugUtilsMessengerCreateInfo.setMessageType(messageType);

        vk::DebugUtilsMessengerEXT debugCallback;
        // TODO: Better manage exceptions
        if (vk::Result::eSuccess != instance.createDebugUtilsMessengerEXT(&debugUtilsMessengerCreateInfo, nullptr, &debugCallback)) throw std::exception {};
        return debugCallback;
    }

} // namespace nuts