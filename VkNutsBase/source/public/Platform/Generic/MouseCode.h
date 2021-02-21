#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <GLFW/glfw3.h>
#include <Utilities/Utilities.hpp>

namespace nuts {
    enum class MouseCode : std::int16_t {
        Unkown = GLFW_KEY_UNKNOWN,

        Button1 = GLFW_MOUSE_BUTTON_1,
        Button2 = GLFW_MOUSE_BUTTON_2,
        Button3 = GLFW_MOUSE_BUTTON_3,
        Button4 = GLFW_MOUSE_BUTTON_4,
        Button5 = GLFW_MOUSE_BUTTON_5,
        Button6 = GLFW_MOUSE_BUTTON_6,
        Button7 = GLFW_MOUSE_BUTTON_7,
        Button8 = GLFW_MOUSE_BUTTON_8,

        ButtonLast   = GLFW_MOUSE_BUTTON_LAST,
        ButtonLeft   = GLFW_MOUSE_BUTTON_LEFT,
        ButtonRight  = GLFW_MOUSE_BUTTON_RIGHT,
        ButtonMiddle = GLFW_MOUSE_BUTTON_MIDDLE,

#if defined(NUTS_ENABLE_LOG)
        // Count tracker
        NUTS_ENUM_COUNT = 13,
#endif
    };

#if defined(NUTS_ENABLE_LOG)
    static constexpr ConstexprMap< MouseCode, const char*, enum_size_v< MouseCode > > MouseCodeToString { { {
        { MouseCode::Unkown, "Unknown" },
        { MouseCode::Button1, "Button1" },
        { MouseCode::Button2, "Button2" },
        { MouseCode::Button3, "Button3" },
        { MouseCode::Button4, "Button4" },
        { MouseCode::Button5, "Button5" },
        { MouseCode::Button6, "Button6" },
        { MouseCode::Button7, "Button7" },
        { MouseCode::Button8, "Button8" },
        { MouseCode::ButtonLast, "ButtonLast" },
        { MouseCode::ButtonLeft, "ButtonLeft" },
        { MouseCode::ButtonRight, "ButtonRight" },
        { MouseCode::ButtonMiddle, "ButtonMiddle" },
    } } };
#endif
} // namespace nuts

#if defined(NUTS_ENABLE_LOG)
inline std::ostream& operator<<(std::ostream& os, const nuts::MouseCode& mouseCode) {
    os << nuts::MouseCodeToString.at(mouseCode);
    return os;
}
#endif