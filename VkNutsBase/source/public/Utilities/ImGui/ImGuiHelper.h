// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <imgui.h>
#include <Platform/Vulkan/VulkanHelper.h>
#include <Platform/Generic/IWindow.h>

namespace nuts {

    class ImGuiHelper {
      public:
        STATIC_CLASS(ImGuiHelper)

        enum class ImGuiHStyle : uint8_t
        {
            Dark,
            Light,
        };
        struct ImGuiInitializer {
            vk::Instance       instance;
            vk::Device         device;
            vk::PhysicalDevice physicalDevice;
            vk::SurfaceKHR     surfaceKHR;
            QueueFamilyIndices graphicsQueueIndex;
            IWindow*           window;
            vk::RenderPass     renderPass;
            vk::Queue          queue;
            uint32_t           frameBufferSize;
            vk::CommandPool    commandPool;
        };

        static void createContext();
        static void init(const ImGuiInitializer& initializer);
        static void finalize() noexcept;

        static void onResize(float width, float height);

        static void beginRender(int width, int height);
        static void render(vk::CommandBuffer commandBuffer);
        static void endRender();

      private:
        static void                setStyle(ImGuiHStyle imGuiHstyle = ImGuiHStyle::Dark);
        static void                setFonts();
        [[nodiscard]] static float getDPIScale();

        static void setStyleDark();
        static void setStyleLight();

        [[nodiscard]] static vk::SurfaceKHR&     getSurfaceKHR() noexcept;
        [[nodiscard]] static vk::Instance&       getInstance() noexcept;
        [[nodiscard]] static vk::PhysicalDevice& getPhysicalDevice() noexcept;
        [[nodiscard]] static vk::Device&         getDevice() noexcept;
        [[nodiscard]] static uint32_t&           getGraphicsQueueIndex() noexcept;
        [[nodiscard]] static IWindow*&           getWindow() noexcept;
        [[nodiscard]] static vk::DescriptorPool& getDescriptorPool() noexcept;
        [[nodiscard]] static vk::RenderPass&     getRenderPass() noexcept;
        [[nodiscard]] static vk::Queue&          getQueue() noexcept;
        [[nodiscard]] static uint32_t&           getFrameBufferSize() noexcept;
        [[nodiscard]] static vk::CommandPool&    getCommandPool() noexcept;
    };

} // namespace nuts
