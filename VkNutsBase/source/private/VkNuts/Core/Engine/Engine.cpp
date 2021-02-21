// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanHelper.h>
#include <VkNuts/Core/Window/Window.h>
#include <VkNuts/Core/Event/WindowEvents.h>
#include <VkNuts/Core/Log/Log.h>
#include <VkNuts/VkRenderer/VkRenderer.h>

#include <VkNuts/Core/Engine/Engine.h>

namespace nuts {
    int Engine::run() {
        Log::Init();
    
        nuts::InstanceData id {};

        id.name       = "test";
        id.validation = nuts::RuntimeValidation::Enable;
        id.minVersion = VK_API_VERSION_1_1;
        id.window     = nullptr;

        auto window = Window::Create();

        window->Init({ 512, 512, "No Title", false, nullptr });

        const char* exten[] = { VK_KHR_SURFACE_EXTENSION_NAME };

        auto instance = nuts::VulkanHelper::CreateInstance(id, nuts::array_size::get< uint32_t >(exten), exten);

        auto phy_dev = nuts::VulkanHelper::SelectPhysicalDevice(instance, nuts::PhsyicalDeviceSelectionStrategy::SelectLatestAPI);

        auto sur = nuts::VulkanHelper::CreateSurface(instance, reinterpret_cast< IWindow* >(window.get()));

        nuts::QueuePopulate q;
        q.flags         = vk::QueueFlagBits::eGraphics;
        q.priority      = 1.f;
        q.surface       = sur;
        q.queueFamilyId = nuts::VulkanHelper::SelectQueueFamilyIndex(phy_dev, sur, vk::QueueFlagBits::eGraphics);

        nuts::DeviceData dd;
        dd.physicalDevice = phy_dev;
        dd.queues         = { q };
        dd.validation     = nuts::RuntimeValidation::Enable;

        auto device = nuts::VulkanHelper::CreateDevice(dd);

        VkRenderer::Init(device);
        VkRenderer::mPhysicalDevice = phy_dev;

        auto debug = nuts::VulkanHelper::CreateDebugUtilsMessenger(instance);

        instance.destroyDebugUtilsMessengerEXT(debug);
        instance.destroySurfaceKHR(sur);
        VkRenderer::Finalize();

        device.destroy();
        instance.destroy();
        window->Finalize();
        return 0;
    }

    int Engine::getValue() { return 5; }
} // namespace nuts