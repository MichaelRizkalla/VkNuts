#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/ConstexprMap.hpp>

namespace nuts {
    enum class EventCategory : std::uint32_t {
        Undefined = 0,

        // Window Events
        Window = 1,

        // Input for both keyboard and mouse
        Input = 2,

        // Keyboard Events
        Keyboard = 4,

        // Mouse Events
        Mouse = 8,

        // Engine Events
        Engine   = 16,
        Graphics = 32,

#if defined(NUTS_ENABLE_LOG)
        // Count tracker
        NUTS_ENUM_COUNT = 7,
#endif
    };

    inline EventCategory operator|(EventCategory lhs, EventCategory rhs) {
        using _UnderlyingType = std::underlying_type_t< EventCategory >;
        return static_cast< EventCategory >(static_cast< _UnderlyingType >(lhs) | static_cast< _UnderlyingType >(rhs));
    }

    inline EventCategory operator&(EventCategory lhs, EventCategory rhs) {
        using _UnderlyingType = std::underlying_type_t< EventCategory >;
        return static_cast< EventCategory >(static_cast< _UnderlyingType >(lhs) & static_cast< _UnderlyingType >(rhs));
    }

#if defined(NUTS_ENABLE_LOG)
    static constexpr ConstexprMap< EventCategory, const char*, enum_size_v< EventCategory > > EventCategoryToString { { {
        { EventCategory::Undefined, "Undefined" },
        { EventCategory::Window, "Window" },
        { EventCategory::Keyboard, "Keyboard" },
        { EventCategory::Mouse, "Mouse" },
        { EventCategory::Engine, "Engine" },
        { EventCategory::Graphics, "Graphics" },
    } } };
#endif
} // namespace nuts

#if defined(NUTS_ENABLE_LOG)
inline std::ostream& operator<<(std::ostream& os, const nuts::EventCategory& eventType) {
    os << nuts::EventCategoryToString.at(eventType);
    return os;
}
#endif