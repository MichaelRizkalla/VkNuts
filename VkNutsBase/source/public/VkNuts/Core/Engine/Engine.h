#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanFwd.h>

#include <Utilities/SingletonCRTP.hpp>
#include <VkNuts/Core/Event/WindowEvents.h>
#include <VkNuts/Core/Window/Window.h>
#include <VkNuts/Core/Layer/LayerRegistry.h>

namespace nuts {
    struct Engine {
      public:
        NUTS_API static Engine* getInstance();

        NUTS_API void run();
        NUTS_API void close();
        NUTS_API ~Engine();

      protected:
        void     onEvent(Event& event_);
        NUTS_API Engine();

      private:
        bool onWindowResize(WindowResizedEvent& windowResizedEvent);
        // NUTS_API bool onWindowClose(WindowClosedEvent& windowClosedEvent);

        UniqueRef< Window > mWindow;
        bool                mRunning;
        bool                mMinimized;
        LayerRegistry       mLayers;
        TimeController      mTimeController;
        uint32_t            mFrameCount;

        void createWindow();
    };

} // namespace nuts
