#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <GLFW/glfw3.h>

namespace nuts {
    struct NUTS_INTERFACE_ONLY IWindow {
        DEFAULT_INTERFACE(IWindow)

#if defined(NUTS_OS_WINDOWS)
        using NativeHandle = HWND;
#elif defined(NUTS_OS_LINUX)
        using NativeHandle = void*;
#else
    #error "OS is not yet supported!"
#endif

        virtual const GLFWwindow*  getHandle() const noexcept       = 0;
        virtual GLFWwindow*        getHandle() noexcept             = 0;
        virtual const NativeHandle getNativeHandle() const noexcept = 0;
        virtual NativeHandle       getNativeHandle() noexcept       = 0;
    };
} // namespace nuts