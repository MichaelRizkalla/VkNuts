#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <GLFW/glfw3.h>
#include <VkNuts/Core/Registry/Registry.h>
#include <VkNuts/Core/Attachment/SharedLibraryAttachment.h>

namespace nuts {

    struct WindowCallbackAttachment final : public SharedLibraryAttachment {
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

        explicit WindowCallbackAttachment(const char* name) : SharedLibraryAttachment { name } {};

        attachment_data getData() const {
            attachment_data data {};
            data.getUsage = reinterpret_cast< attachment_data::UsageSignature >(FunctionLoader::load(mHandle, "getUsage"));

            if (!data.getUsage) {
                NUTS_ENGINE_ERROR("Failed to obtain function getUsage()'s pointer!");
                NUTS_ENGINE_ERROR("Please unload the attachment to avoid further exceptions!");

                // TODO: Better handle exceptions
                throw std::exception {};
            }
            if (std::strcmp(data.getUsage(), "WindowCallbackAttachment") != 0) {
                NUTS_ENGINE_ERROR("Trying to use attachment of type {} using WindowCallbackRegistry", data.getUsage());
                NUTS_ENGINE_ERROR("Please unload the attachment to avoid further exceptions!");

                // TODO: Better handle exceptions
                throw std::exception {};
            }

            data.charIOFunc         = reinterpret_cast< attachment_data::CharIOSignature >(FunctionLoader::load(mHandle, "OnChar"));
            data.cursorPositionFunc = reinterpret_cast< attachment_data::CursorPositionSignature >(FunctionLoader::load(mHandle, "OnCursorPosition"));
            data.keyIOFunc          = reinterpret_cast< attachment_data::KeyIOSignature >(FunctionLoader::load(mHandle, "OnKey"));
            data.mouseButtonFunc    = reinterpret_cast< attachment_data::MouseButtonSignature >(FunctionLoader::load(mHandle, "OnMouseButton"));
            data.mouseScrollFunc    = reinterpret_cast< attachment_data::MouseScrollSignature >(FunctionLoader::load(mHandle, "OnMouseScroll"));
            data.windowCloseFunc    = reinterpret_cast< attachment_data::WindowCloseSignature >(FunctionLoader::load(mHandle, "OnWindowClose"));
            data.windowResizeFunc   = reinterpret_cast< attachment_data::WindowReSizeSignature >(FunctionLoader::load(mHandle, "OnWindowReSize"));

            return data;
        }
    };

    class WindowCallbackRegistry final : public Registry< WindowCallbackAttachment > {
      public:
        WindowCallbackRegistry()  = default;
        ~WindowCallbackRegistry() = default;

        void init(const RegistryInitializer* const) noexcept override {
        }
    };

} // namespace nuts