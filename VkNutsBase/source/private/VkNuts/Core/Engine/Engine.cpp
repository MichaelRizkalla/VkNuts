// clang-format off
// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanHelper.h>
#include <VkNuts/Core/Log/Log.h>
#include <VkNuts/Core/Window/Window.h>

#include <VkNuts/Core/Event/Event.h>
#include <VkNuts/Core/Event/WindowEvents.h>

#include <Platform/Vulkan/VkManagement/CommandBuffer.h>
#include <Platform/Vulkan/VkManagement/FrameBuffer.h>
#include <Platform/Vulkan/VkManagement/RenderPass.h>
#include <Platform/Vulkan/VkManagement/SwapChain.h>
#include <Utilities/ImGui/ImGuiHelper.h>
#include <VkNuts/VkRenderer/VkRenderer.h>

#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>
#include <VkNuts/Core/Engine/Engine.h>

#include <VkNuts/Core/Gui/EditingLayer.h>
#include <glm/glm.hpp>

namespace nuts { namespace helper {

    vk::Instance createInstance() {
        nuts::InstanceData instanceData {};

        instanceData.name       = "VkNuts";
        instanceData.validation = nuts::RuntimeValidation::Enable;
        instanceData.minVersion = VK_API_VERSION_1_2;
        instanceData.window     = nullptr;

        // TODO: make as a parameter
        const char* exten[] = { VK_KHR_SURFACE_EXTENSION_NAME };

        return nuts::VulkanHelper::createInstance(instanceData, nuts::array_size::get< uint32_t >(exten), exten);
    }
    vk::PhysicalDevice createPhysicalDevice(vk::Instance instance) {
        return nuts::VulkanHelper::selectPhysicalDevice(instance, nuts::PhsyicalDeviceSelectionStrategy::SelectLatestAPI);
    }
    vk::SurfaceKHR createSurface(vk::Instance instance, IWindow* window) {
        return nuts::VulkanHelper::createSurface(instance, window);
    }
    std::pair< vk::Device, QueueFamilyIndices > createDeviceAndQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
        nuts::QueuePopulate queuePopulate {};
        queuePopulate.flags         = vk::QueueFlagBits::eGraphics;
        queuePopulate.priority      = 1.f;
        queuePopulate.surface       = surface;
        queuePopulate.queueFamilyId = nuts::VulkanHelper::selectQueueFamilyIndex(physicalDevice, surface, vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute);

        nuts::DeviceData deviceData {};
        deviceData.physicalDevice = physicalDevice;
        deviceData.queues         = { queuePopulate };
        deviceData.validation     = nuts::RuntimeValidation::Enable;

        return std::make_pair(nuts::VulkanHelper::createDevice(deviceData), queuePopulate.queueFamilyId);
    }
}} // namespace nuts::helper

namespace nuts {
    static vk::DebugUtilsMessengerEXT debugMessenger {};

    Engine* Engine::getInstance() {
        struct EngineMaker : Engine {};
        static UniqueRef< Engine > mEngine = UniqueRef< Engine >(new EngineMaker);

        return mEngine.get();
    }

    Engine::Engine() : mMinimized(false), mRunning(true) {
        Log::init();

        createWindow();
        auto instance       = helper::createInstance();
        auto physicalDevice = helper::createPhysicalDevice(instance);
        auto surface        = helper::createSurface(instance, mWindow.get());

        auto [device, queueFamilyIndices] = helper::createDeviceAndQueueFamilyIndex(physicalDevice, surface);

        // TODO: Init should create and manage device, instance, and physical device
        VkRenderer::init(instance, device, physicalDevice, surface, queueFamilyIndices, mWindow.get());

        mLayers.init(nullptr);
        ImGuiHelper::createContext();

        auto editorLayer =
            UniqueRef< Layer >(new EditingLayer("EditingLayer", device, mWindow.get(), VkRenderer::getSwapChainImageFormat()));
        mLayers.attachAttachment("EditingLayer", std::move(editorLayer));
        auto renderPass = mLayers.getAttachment("EditingLayer")->getRenderPass()->getVkHandle();

        VkRenderer::createFrameBuffer(renderPass);

        VkRenderer::createGraphicsCommandPool(queueFamilyIndices);
        VkRenderer::createGraphicsCommandBuffers();

        ImGuiHelper::init({ instance, device, physicalDevice, surface, queueFamilyIndices, mWindow.get(), renderPass, VkRenderer::getGraphicsQueue(),
                            static_cast< uint32_t >(VkRenderer::getSwapChainImageCount()), VkRenderer::getGraphicsCommandPool() });
    }

    Engine::~Engine() {
        Log::finalize();
    }

    void Engine::run() {
        mFrameCount = 0;
        std::array< float, 4 > clearColor { 1.0f, 1.0f, 1.0f, 1.0f };

        std::array< vk::ClearValue, 2 > clearValues {};
        clearValues[0].color        = vk::ClearColorValue { clearColor };
        clearValues[1].depthStencil = vk::ClearDepthStencilValue { 1.0f, 0 };

        mTimeController.start();
        while (mRunning && !glfwWindowShouldClose(mWindow->getHandle())) {

            auto timeStep = mTimeController.end();
            mTimeController.start();
            if (!mMinimized) {
                const auto& layers = mLayers.getAttachments();

                for (auto& layer : layers) {
                    layer.second->onUpdate(timeStep);
                }

                VkRenderer::prepareNextFrame();

                auto dims = mWindow->getDimensions();
                ImGuiHelper::beginRender(dims.first, dims.second);

                VkRenderer::beginGraphicsCommandBuffer(mFrameCount);

                vk::RenderPassBeginInfo renderPassBeginInfo {};
                renderPassBeginInfo.clearValueCount = 2;
                renderPassBeginInfo.pClearValues    = clearValues.data();
                // Get the correct framebuffer attached with the active image in the swap chain
                renderPassBeginInfo.framebuffer = VkRenderer::getActiveFrameBuffer();
                renderPassBeginInfo.renderArea  = vk::Rect2D { { 0, 0 }, VkRenderer::getSwapChainData().extent };

                for (auto& layer : layers) {
                    renderPassBeginInfo.renderPass = layer.second->getRenderPass()->getVkHandle();
                    layer.second->onRender(VkRenderer::getActiveGraphicsCommandBuffer(mFrameCount));
                    VkRenderer::beginGraphicsRenderPass(mFrameCount, renderPassBeginInfo);
                    ImGuiHelper::render(VkRenderer::getActiveGraphicsCommandBuffer(mFrameCount).getVkHandle());
                    VkRenderer::endGraphicsRenderPass(mFrameCount);
                }

                ImGuiHelper::endRender();

                VkRenderer::endGraphicsCommandBuffer(mFrameCount);

                VkRenderer::submitGraphicsCommandBuffer(mFrameCount, { vk::PipelineStageFlagBits::eColorAttachmentOutput });
                VkRenderer::present();

                VkRenderer::deviceWaitIdle();

                ++mFrameCount;
            }

            mWindow->onUpdate();
        }

        // Cleanup
        mLayers.detachAllAttachments();

        ImGuiHelper::finalize();
        VkRenderer::finalize();
        mWindow->finalize();
    }

    void Engine::onEvent(Event& event_) {
        (void)event_;

        EventHandler eventHandler { event_ };

        eventHandler.handle< WindowResizedEvent >([this](WindowResizedEvent& e) { return Engine::getInstance()->onWindowResize(e); });

        const auto& layers = mLayers.getAttachments();
        for (auto& layer : layers) {
            layer.second->onEvent(event_);
        }
    }

    void Engine::close() {
        mRunning = false;
    }

    void Engine::createWindow() {
        mWindow = Window::create();

        if (!mWindow->init({ 1600, 900, "VkNuts", false, nullptr })) {
            NUTS_ENGINE_ERROR("Failed to create the window");
        }

        mWindow->setEventCallback([this](Event& e) { Engine::getInstance()->onEvent(e); });
    }

    bool Engine::onWindowResize(WindowResizedEvent& windowResizedEvent) {
        auto [width, height] = windowResizedEvent.getDimensions();
        if (width == 0 || height == 0) {
            mMinimized = true;
            return true;
        }
        mMinimized = false;

        ImGuiHelper::onResize(static_cast< float >(width), static_cast< float >(height));

        VkRenderer::deviceWaitIdle();
        VkRenderer::getGraphicsQueue().waitIdle();
        VkRenderer::getPresentationQueue().waitIdle();

        VkRenderer::updateRenderTarget();

        return true;
    }

} // namespace nuts
