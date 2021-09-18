// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Layer/Layer.h>
#include <Platform/Generic/IWindow.h>
#include <Platform/Vulkan/VulkanFwd.h>

namespace nuts {

    struct ImGuiLayer : public Layer {

        explicit ImGuiLayer(const char* name, vk::Device device, IWindow* window, vk::Format swapChainImageFormat, vk::Format depthImageFormat);

        [[nodiscard]] bool                    onLoad() noexcept override;
        bool                                  onUnload() noexcept override;
        [[nodiscard]] const RenderPass* const getRenderPass() const noexcept override;

      private:
        [[nodiscard]] static std::atomic_bool&   getImGuiImplInit() noexcept;
        [[nodiscard]] static std::atomic_size_t& getLayerCount() noexcept;

      protected:
        static UniqueRef< RenderPass > mRenderPass;
        vk::Device                     mDevice;
        vk::Format                     mImageFormat;
        vk::Format                     mDepthFormat;

      private:
        vk::AttachmentDescription              createAttachmentDescription(vk::Format imageFormat) const noexcept;
        std::array< vk::SubpassDependency, 2 > createSubpassDependency() const noexcept;
    };

} // namespace nuts
