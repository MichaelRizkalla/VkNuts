#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <GLFW/glfw3.h>
#include <VkNuts/Core/Log/Log.h>

namespace nuts {

    struct WindowCallbacks final {
        struct WindowCallbackAttachmentData {
            using UsageSignature = const char* (*)();

            using CharIOSignature         = void (*)(GLFWwindow* window, unsigned int keyCode);
            using CursorPositionSignature = void (*)(GLFWwindow* window, double xPos, double yPos);
            using KeyIOSignature          = void (*)(GLFWwindow* window, int key, int scanCode, int action, int mods);
            using MouseButtonSignature    = void (*)(GLFWwindow* window, int button, int action, int mods);
            using MouseScrollSignature    = void (*)(GLFWwindow* window, double xOffset, double yOffset);
            using WindowCloseSignature    = void (*)(GLFWwindow* window);
            using WindowReSizeSignature   = void (*)(GLFWwindow* window, int width, int height);

            CharIOSignature         charIOFunc;
            CursorPositionSignature cursorPositionFunc;
            KeyIOSignature          keyIOFunc;
            MouseButtonSignature    mouseButtonFunc;
            MouseScrollSignature    mouseScrollFunc;
            WindowCloseSignature    windowCloseFunc;
            WindowReSizeSignature   windowResizeFunc;
            UsageSignature          getUsage;
        };

        using attachment_data = WindowCallbackAttachmentData;

      private:
        static void onChar(GLFWwindow* window, unsigned int keyCode);
        static void onCursorPosition(GLFWwindow* window, double xPos, double yPos);
        static void onKey(GLFWwindow* window, int key, int scanCode, int action, int mods);
        static void onMouseButton(GLFWwindow* window, int button, int action, int mods);
        static void onMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
        static void onWindowClose(GLFWwindow* window);
        static void onWindowReSize(GLFWwindow* window, int width, int height);

      public:
        static [[nodiscard]] attachment_data getData() noexcept;
    };

} // namespace nuts
