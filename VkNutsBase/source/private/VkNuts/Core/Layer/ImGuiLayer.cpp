// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Layer/ImGuiLayer.h>
#include <Utilities/ImGui/imgui/backends/imgui_impl_glfw.h>
#include <Platform/Vulkan/VkManagement/RenderPass.h>

namespace nuts {

    UniqueRef< RenderPass > ImGuiLayer::mRenderPass = nullptr;

    ImGuiLayer::ImGuiLayer(const char* name, vk::Device device, IWindow* window, vk::Format swapChainImageFormat, vk::Format depthImageFormat) :
        Layer { name, window }, mImageFormat(swapChainImageFormat), mDevice(device), mDepthFormat(depthImageFormat) {};
    bool ImGuiLayer::onLoad() noexcept {
        bool expected = false;
        bool result   = false;
        if (std::atomic_compare_exchange_strong(&(getImGuiImplInit()), &expected, true)) {
            result = ImGui_ImplGlfw_InitForVulkan(mWindow->getHandle(), false);

            try {
                mRenderPass = std::make_unique< RenderPass >();
                mRenderPass->init(mDevice);

                vk::AttachmentDescription swapChainAttachmentDescription = createAttachmentDescription(mImageFormat);

                vk::AttachmentReference colorAttachmentReference {};
                colorAttachmentReference.attachment = 0;
                colorAttachmentReference.layout     = vk::ImageLayout::eColorAttachmentOptimal;

                std::array< vk::SubpassDescription, 1 > subpass {};
                subpass[0].pipelineBindPoint    = vk::PipelineBindPoint::eGraphics;
                subpass[0].colorAttachmentCount = 1;
                subpass[0].pColorAttachments    = &colorAttachmentReference;

                auto subpassDependency = createSubpassDependency();

                std::array< vk::AttachmentDescription, 1 > attachmentDescriptions = { swapChainAttachmentDescription };

                mRenderPass->createRenderPass({ mImageFormat }, mDepthFormat, 1, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR, false, false);

            } catch (...) {
                if (mRenderPass) {
                    mRenderPass.reset(nullptr);
                }
                return false;
            }
        }
        ++getLayerCount();
        
        return result;
    }
    bool ImGuiLayer::onUnload() noexcept {
        auto value = --getLayerCount();

        if (value == 0) {
            // ImGui_ImplVulkan_Shutdown();
            mRenderPass->finalize();
            mRenderPass = nullptr;
        }
        return true;
    }
    const RenderPass* const ImGuiLayer::getRenderPass() const noexcept {
        return mRenderPass.get();
    }
    std::atomic_bool& ImGuiLayer::getImGuiImplInit() noexcept {
        static std::atomic_bool isInit = false;

        return isInit;
    }
    std::atomic_size_t& ImGuiLayer::getLayerCount() noexcept {
        static std::atomic_size_t layerCount = 0;

        return layerCount;
    }

    vk::AttachmentDescription ImGuiLayer::createAttachmentDescription(vk::Format imageFormat) const noexcept {
        vk::AttachmentDescription attachmentDescription {};

        attachmentDescription.format         = imageFormat;
        attachmentDescription.samples        = vk::SampleCountFlagBits::e1;
        attachmentDescription.loadOp         = vk::AttachmentLoadOp::eClear;
        attachmentDescription.storeOp        = vk::AttachmentStoreOp::eStore;
        attachmentDescription.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;
        attachmentDescription.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachmentDescription.initialLayout  = vk::ImageLayout::eUndefined;
        attachmentDescription.finalLayout    = vk::ImageLayout::ePresentSrcKHR;

        return attachmentDescription;
    }
    std::array< vk::SubpassDependency, 2 > ImGuiLayer::createSubpassDependency() const noexcept {
        std::array< vk::SubpassDependency, 2 > subpassDependencies {};

        subpassDependencies[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
        subpassDependencies[0].srcStageMask    = vk::PipelineStageFlagBits::eBottomOfPipe;
        subpassDependencies[0].srcAccessMask   = vk::AccessFlagBits::eMemoryRead;
        subpassDependencies[0].dstSubpass      = 0;
        subpassDependencies[0].dstStageMask    = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        subpassDependencies[0].dstAccessMask   = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        subpassDependencies[0].dependencyFlags = static_cast< vk::DependencyFlagBits >(0);

        subpassDependencies[1].srcSubpass      = 0;
        subpassDependencies[1].srcStageMask    = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        subpassDependencies[1].srcAccessMask   = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        subpassDependencies[1].dstSubpass      = VK_SUBPASS_EXTERNAL;
        subpassDependencies[1].dstStageMask    = vk::PipelineStageFlagBits::eBottomOfPipe;
        subpassDependencies[1].dstAccessMask   = vk::AccessFlagBits::eMemoryRead;
        subpassDependencies[1].dependencyFlags = static_cast< vk::DependencyFlagBits >(0);

        return subpassDependencies;
    }

} // namespace nuts
