#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanFwd.h>

#include <VkNuts/Core/Event/Event.h>
#include <VkNuts/Core/Attachment/Attachment.h>
#include <Utilities/TimeController.hpp>
#include <Platform/Generic/IWindow.h>

namespace nuts {

    struct Layer : Attachment {
      public:
        explicit Layer(const char* name, IWindow* window) : Attachment { name }, mWindow(window) {};

        virtual void                    onUpdate(TimeController::time_step_type& ts) = 0;
        virtual void                    onRender(CommandBuffer& commandBuffer)       = 0;
        virtual void                    onEvent(Event& event_)                       = 0;
        [[nodiscard]] virtual const RenderPass* const getRenderPass() const noexcept               = 0;

      protected:
        IWindow* mWindow;
    };

} // namespace nuts
