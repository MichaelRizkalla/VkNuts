#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <GLFW/glfw3.h>
#include <VkNuts/Core/Registry/Registry.h>
#include <VkNuts/Core/Attachment/Attachment.h>

namespace nuts {
    struct WindowCallbackAttachmentData {
        using UsageSignature = const char* (*)();

        using CharIOSignature         = void (*)(GLFWwindow* window, unsigned int keyCode);
        using CursorPositionSignature = void (*)(GLFWwindow* window, double xPos, double yPos);
        using KeyIOSignature          = void (*)(GLFWwindow* window, int key, int scanCode, int action, int mods);
        using MouseButtonSignature    = void (*)(GLFWwindow* window, int button, int action, int mods);
        using MouseScrollSignature    = void (*)(GLFWwindow* window, double xOffset, double yOffset);
        using WindowCloseSignature    = void (*)(GLFWwindow* window);
        using WindowReSizeSignature   = void (*)(GLFWwindow* window, int width, int height);

        CharIOSignature         CharIOFunc;
        CursorPositionSignature CursorPositionFunc;
        KeyIOSignature          KeyIOFunc;
        MouseButtonSignature    MouseButtonFunc;
        MouseScrollSignature    MouseScrollFunc;
        WindowCloseSignature    WindowCloseFunc;
        WindowReSizeSignature   WindowResizeFunc;
        UsageSignature          GetUsage;
    };

    struct WindowCallbackAttachment final : public Attachment< WindowCallbackAttachmentData > {
        WindowCallbackAttachment(const char* name) : Attachment { name } {};
        ~WindowCallbackAttachment() = default;

        attachment_data GetAttachmentData() const override {
            WindowCallbackAttachmentData data {};
            data.GetUsage = reinterpret_cast< WindowCallbackAttachmentData::UsageSignature >(FunctionLoader::load(mHandle, "GetUsage"));

            if (std::strcmp(data.GetUsage(), "WindowCallbackAttachment") != 0) {
                NUTS_ENGINE_ERROR("Trying to use attachment of type {} using WindowCallbackRegistry", data.GetUsage());
                NUTS_ENGINE_ERROR("Please unload the attachment to avoid further exceptions!");

                // Better handle exceptions
                throw std::exception {};
            }

            data.CharIOFunc         = reinterpret_cast< WindowCallbackAttachmentData::CharIOSignature >(FunctionLoader::load(mHandle, "OnChar"));
            data.CursorPositionFunc = reinterpret_cast< WindowCallbackAttachmentData::CursorPositionSignature >(FunctionLoader::load(mHandle, "OnCursorPosition"));
            data.KeyIOFunc          = reinterpret_cast< WindowCallbackAttachmentData::KeyIOSignature >(FunctionLoader::load(mHandle, "OnKey"));
            data.MouseButtonFunc    = reinterpret_cast< WindowCallbackAttachmentData::MouseButtonSignature >(FunctionLoader::load(mHandle, "OnMouseButton"));
            data.MouseScrollFunc    = reinterpret_cast< WindowCallbackAttachmentData::MouseScrollSignature >(FunctionLoader::load(mHandle, "OnMouseScroll"));
            data.WindowCloseFunc    = reinterpret_cast< WindowCallbackAttachmentData::WindowCloseSignature >(FunctionLoader::load(mHandle, "OnWindowClose"));
            data.WindowResizeFunc   = reinterpret_cast< WindowCallbackAttachmentData::WindowReSizeSignature >(FunctionLoader::load(mHandle, "OnWindowReSize"));

            return data;
        }
    };

    class WindowCallbackRegistry final : public Registry< WindowCallbackAttachment > {
      public:
        WindowCallbackRegistry() = default;
        ~WindowCallbackRegistry() = default;

        void Init(RegistryInitializer*) noexcept override {}

        attachment_type::attachment_data QueryAttachment(const char* alias) const override {
            if (!mRegistry.contains(alias)) {
                NUTS_ENGINE_WARN("An attachment with name {} is not loaded!", alias);
                return attachment_type::attachment_data {};
            }
            // GetAttachmentData might throw
            auto mData = mRegistry.at(alias)->GetAttachmentData();
            return mData;
        }
    };

} // namespace nuts