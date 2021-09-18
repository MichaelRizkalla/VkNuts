#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Event/Event.h>

namespace nuts {
    struct WindowProperties {
        int                           Width;
        int                           Height;
        const char*                   Title;
        bool                          VSync;
        std::function< void(Event&) > EventCallback;
    };
} // namespace nuts
