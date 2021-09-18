// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/ImGui/ImGuiHelper.h>
#include <Utilities/ImGui/imgui/backends/imgui_impl_vulkan.h>
#include <Utilities/ImGui/imgui/backends/imgui_impl_glfw.h>

namespace nuts {

    void ImGuiHelper::createContext() {
        // UI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }

    void ImGuiHelper::init(const ImGuiInitializer& initializer) {
        getInstance()           = initializer.instance;
        getPhysicalDevice()     = initializer.physicalDevice;
        getDevice()             = initializer.device;
        getSurfaceKHR()         = initializer.surfaceKHR;
        getGraphicsQueueIndex() = initializer.graphicsQueueIndex.graphicsFamily.has_value();
        getWindow()             = initializer.window;
        getRenderPass()         = initializer.renderPass;
        getQueue()              = initializer.queue;
        getFrameBufferSize()    = initializer.frameBufferSize;
        getCommandPool()        = initializer.commandPool;

        ImGuiIO& io    = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGuiHelper::setStyle();
        ImGuiHelper::setFonts();

        std::vector< vk::DescriptorPoolSize > poolSize { vk::DescriptorPoolSize { vk::DescriptorType::eSampler, 1 },
                                                         vk::DescriptorPoolSize { vk::DescriptorType::eCombinedImageSampler, 1 } };
        vk::DescriptorPoolCreateInfo          poolInfo {};
        poolInfo.maxSets       = 2;
        poolInfo.poolSizeCount = 2;
        poolInfo.pPoolSizes    = poolSize.data();
        if (vk::Result::eSuccess != getDevice().createDescriptorPool(&poolInfo, nullptr, &getDescriptorPool())) {
            throw std::exception { "Failed to create DescriptorPool!" };
        }

        // Setup Platform/Renderer back ends
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance                  = getInstance();
        init_info.PhysicalDevice            = getPhysicalDevice();
        init_info.Device                    = getDevice();
        init_info.QueueFamily               = getGraphicsQueueIndex();
        init_info.Queue                     = getQueue();
        init_info.PipelineCache             = VK_NULL_HANDLE;
        init_info.DescriptorPool            = getDescriptorPool();
        init_info.Subpass                   = 0;
        init_info.MinImageCount             = 2;
        init_info.ImageCount                = static_cast< uint32_t >(getFrameBufferSize());
        init_info.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;
        init_info.CheckVkResultFn           = nullptr;
        init_info.Allocator                 = nullptr;
        ImGui_ImplVulkan_Init(&init_info, getRenderPass());

        // Upload Fonts
        vk::CommandBufferAllocateInfo allocateInfo {};
        allocateInfo.commandBufferCount = 1;
        allocateInfo.commandPool        = getCommandPool();
        allocateInfo.level              = vk::CommandBufferLevel::ePrimary;
        vk::CommandBuffer cmdbuf;
        if (vk::Result::eSuccess != getDevice().allocateCommandBuffers(&allocateInfo, &cmdbuf)) {
            throw std::exception { "Failed to create CommandBuffers!" };
        }

        vk::CommandBufferBeginInfo beginInfo {};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        cmdbuf.begin(beginInfo);

        ImGui_ImplVulkan_CreateFontsTexture(cmdbuf);

        cmdbuf.end();

        vk::SubmitInfo submitInfo {};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &cmdbuf;
        getQueue().submit(submitInfo);
        getQueue().waitIdle();
        getDevice().freeCommandBuffers(getCommandPool(), cmdbuf);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
    void ImGuiHelper::finalize() noexcept {
        if (getDescriptorPool()) {
            getDevice().destroyDescriptorPool(getDescriptorPool());
        }

        if (ImGui::GetCurrentContext() != nullptr) {
            ImGui_ImplVulkan_Shutdown();
            ImGui::DestroyContext();
        }
    }

    void ImGuiHelper::onResize(float width, float height) {
        if (ImGui::GetCurrentContext() != nullptr) {
            auto& imgui_io       = ImGui::GetIO();
            imgui_io.DisplaySize = ImVec2(static_cast< float >(width), static_cast< float >(height));
        }
    }

    void ImGuiHelper::beginRender(int width, int height) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)), 0);
    }
    void ImGuiHelper::render(vk::CommandBuffer commandBuffer) {
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }
    void ImGuiHelper::endRender() {
    }

    void ImGuiHelper::setStyle(ImGuiHStyle imGuiHstyle) {
        switch (imGuiHstyle) {
            case ImGuiHStyle::Dark: {
                ImGuiHelper::setStyleDark();
            } break;
            case ImGuiHStyle::Light: {
                ImGuiHelper::setStyleLight();
            } break;
            default: {
                // TODO: Better exception management
                throw std::exception { "Invalid ImGui Style" };
            }
        }
    }
    void ImGuiHelper::setFonts() {
        ImGuiIO&    io             = ImGui::GetIO();
        const float high_dpi_scale = getDPIScale();

        // Nicer fonts
        ImFont*     font    = nullptr;
        const char* vk_path = getenv("VK_SDK_PATH");
        if (vk_path) {
            const std::string p = std::string(vk_path) + R"(/Samples/Layer-Samples/data/FreeSans.ttf)";
            if (std::filesystem::exists(p.c_str()))
                font = io.Fonts->AddFontFromFileTTF(p.c_str(), 16.0f * high_dpi_scale);
        }
        if (font == nullptr) {
            const std::string p = R"(C:/Windows/Fonts/segoeui.ttf)";
            if (std::filesystem::exists(p.c_str()))
                font = io.Fonts->AddFontFromFileTTF(p.c_str(), 16.0f * high_dpi_scale);
        }
        if (font == nullptr) {
            const std::string p = "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf";
            if (std::filesystem::exists(p.c_str()))
                font = io.Fonts->AddFontFromFileTTF(p.c_str(), 16.0f * high_dpi_scale);
        }
    }
    float ImGuiHelper::getDPIScale() {
        // Cached DPI scale, so that this doesn't change after the first time code calls getDPIScale.
        // A negative value indicates that the value hasn't been computed yet.
        static float cached_dpi_scale = 1.0f;

        return cached_dpi_scale;
    }

    void ImGuiHelper::setStyleDark() {
        ImGui::StyleColorsDark();
    }
    void ImGuiHelper::setStyleLight() {
        ImGui::StyleColorsLight();
    }

    vk::SurfaceKHR& ImGuiHelper::getSurfaceKHR() noexcept {
        static vk::SurfaceKHR mSurfaceKHR { nullptr };

        return mSurfaceKHR;
    }
    vk::Instance& ImGuiHelper::getInstance() noexcept {
        static vk::Instance mInstance { nullptr };

        return mInstance;
    }
    vk::PhysicalDevice& ImGuiHelper::getPhysicalDevice() noexcept {
        static vk::PhysicalDevice mPhysicalDevice { nullptr };

        return mPhysicalDevice;
    }
    vk::Device& ImGuiHelper::getDevice() noexcept {
        static vk::Device mDevice { nullptr };

        return mDevice;
    }
    uint32_t& ImGuiHelper::getGraphicsQueueIndex() noexcept {
        static uint32_t mGraphicsQueueIndex { 0 };

        return mGraphicsQueueIndex;
    }
    IWindow*& ImGuiHelper::getWindow() noexcept {
        static IWindow* mWindow { nullptr };

        return mWindow;
    }
    vk::DescriptorPool& ImGuiHelper::getDescriptorPool() noexcept {
        static vk::DescriptorPool mDescriptorPool { nullptr };

        return mDescriptorPool;
    }
    vk::RenderPass& ImGuiHelper::getRenderPass() noexcept {
        static vk::RenderPass mRenderPass { nullptr };

        return mRenderPass;
    }
    vk::Queue& ImGuiHelper::getQueue() noexcept {
        static vk::Queue mQueue { nullptr };

        return mQueue;
    }
    uint32_t& ImGuiHelper::getFrameBufferSize() noexcept {
        static uint32_t mFramebufferSize { 0 };

        return mFramebufferSize;
    }
    vk::CommandPool& ImGuiHelper::getCommandPool() noexcept {
        static vk::CommandPool mCommandPool { nullptr };

        return mCommandPool;
    }

} // namespace nuts
