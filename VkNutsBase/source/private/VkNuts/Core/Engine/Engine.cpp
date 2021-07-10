// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Log/Log.h>
#include <Platform/Vulkan/VulkanHelper.h>
#include <VkNuts/Core/Window/Window.h>
#include <VkNuts/Core/Event/WindowEvents.h>
#include <VkNuts/VkRenderer/VkRenderer.h>

#include <VkNuts/Core/Engine/Engine.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>

namespace nuts {
    int Engine::run() {
        Log::init();

        {
            nuts::InstanceData id {};

            id.name       = "test";
            id.validation = nuts::RuntimeValidation::Enable;
            id.minVersion = VK_API_VERSION_1_1;
            id.window     = nullptr;

            auto window = Window::create();

            window->init({ 512, 512, "Test Run", false, nullptr });

            auto second_window = Window::create();

            second_window->init({ 480, 320, "Second window", true, nullptr });

            const char* exten[] = { VK_KHR_SURFACE_EXTENSION_NAME };

            auto instance = nuts::VulkanHelper::createInstance(id, nuts::array_size::get< uint32_t >(exten), exten);

            auto phy_dev = nuts::VulkanHelper::selectPhysicalDevice(instance, nuts::PhsyicalDeviceSelectionStrategy::SelectLatestAPI);

            auto sur = nuts::VulkanHelper::createSurface(instance, reinterpret_cast< IWindow* >(window.get()));

            nuts::QueuePopulate q;
            q.flags         = vk::QueueFlagBits::eGraphics;
            q.priority      = 1.f;
            q.surface       = sur;
            q.queueFamilyId = nuts::VulkanHelper::selectQueueFamilyIndex(phy_dev, sur, vk::QueueFlagBits::eGraphics);

            nuts::DeviceData dd;
            dd.physicalDevice = phy_dev;
            dd.queues         = { q };
            dd.validation     = nuts::RuntimeValidation::Enable;

            auto device = nuts::VulkanHelper::createDevice(dd);

            VmaAllocatorCreateInfo allocatorCI {};
            allocatorCI.instance       = instance;
            allocatorCI.device         = device;
            allocatorCI.physicalDevice = phy_dev;

            VkRenderer::init(instance, device, phy_dev);

            VkRenderer::loadShader("vert.spv", "vertex_pass_through");
            VkRenderer::loadShader("frag.spv", "fragment_pass_through");
            if (!VkRenderer::createShader("vertex_pass_through")) { NUTS_ENGINE_ERROR("Failed to create shader: ", "vertex_pass_through"); }
            if (!VkRenderer::createShader("fragment_pass_through")) { NUTS_ENGINE_ERROR("Failed to create shader: ", "fragment_pass_through"); }

            auto debug = nuts::VulkanHelper::createDebugUtilsMessenger(instance);

            instance.destroyDebugUtilsMessengerEXT(debug);
            instance.destroySurfaceKHR(sur);

            VkRenderer::finalize();

            device.destroy();
            instance.destroy();
            window->finalize();
            second_window->finalize();
        }

        Log::finalize();
        return 0;
    }

    int Engine::getValue() { return 5; }
} // namespace nuts