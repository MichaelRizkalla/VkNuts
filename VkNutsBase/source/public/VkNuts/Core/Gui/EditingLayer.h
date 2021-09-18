// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Layer/ImGuiLayer.h>

namespace nuts {

    struct EditingLayer : ImGuiLayer {

        explicit EditingLayer(const char* name, vk::Device device, IWindow* window, vk::Format swapChainImageFormat, vk::Format depthImageFormat);

        void               onUpdate(TimeController::time_step_type& ts) override;
        void               onRender(CommandBuffer& commandBuffer) override;
        void               onEvent(Event& event_) override;

      private:
        void NewScene();
        void OpenScene();
        void SaveAsScene();
    };

} // namespace nuts
