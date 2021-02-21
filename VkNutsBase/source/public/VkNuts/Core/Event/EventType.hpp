#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/ConstexprMap.hpp>

namespace nuts {
    enum class EventType : std::uint32_t {
        Undefined,

        // Window Events
        WindowClosed,
        WindowMinimized,
        WindowResized,
        WindowFocused,
        WindowLost,
        WindowMoved,

        // Keyboard Events
        KeyPressed,
        KeyReleased,
        KeyCharacter,

        // Mouse Events
        MouseButtonPressed,
        MouseButtonReleased,
        MouseScrollTriggered,
        MouseMoved,

        // Engine Events
        ComponentLoaded,
        RegistryLoaded,
        RenderStarted,
        RenderFinished,

#if defined(NUTS_ENABLE_LOG)
        // Count tracker
        NUTS_ENUM_COUNT,
#endif
    };

#if defined(NUTS_ENABLE_LOG)
    static constexpr ConstexprMap< EventType, const char*, enum_size_v< EventType > > EventTypeToString { { {
        { EventType::Undefined, "Undefined" },
        { EventType::WindowClosed, "WindowClosed" },
        { EventType::WindowMinimized, "WindowMinimized" },
        { EventType::WindowResized, "WindowResized" },
        { EventType::WindowFocused, "WindowFocused" },
        { EventType::WindowLost, "WindowLost" },
        { EventType::WindowMoved, "WindowMoved" },
        { EventType::KeyPressed, "KeyPressed" },
        { EventType::KeyReleased, "KeyReleased" },
        { EventType::KeyCharacter, "KeyCharacter" },
        { EventType::MouseButtonPressed, "MouseButtonPressed" },
        { EventType::MouseButtonReleased, "MouseButtonReleased" },
        { EventType::MouseScrollTriggered, "MouseScrollTriggered" },
        { EventType::MouseMoved, "MouseMoved" },
        { EventType::ComponentLoaded, "ComponentLoaded" },
        { EventType::RegistryLoaded, "RegistryLoaded" },
        { EventType::RenderStarted, "RenderStarted" },
        { EventType::RenderFinished, "RenderFinished" },
    } } };
#endif
} // namespace nuts

#if defined(NUTS_ENABLE_LOG)
inline std::ostream& operator<<(std::ostream& os, const nuts::EventType& eventType) {
    os << nuts::EventTypeToString.at(eventType);
    return os;
}
#endif