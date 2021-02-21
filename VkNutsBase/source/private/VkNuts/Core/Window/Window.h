#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <GLFW/glfw3.h>
#include <Utilities/Utilities.hpp>
#include <Platform/Vulkan/IWindow.h>
#include <VkNuts/Core/Event/Event.h>
#include <VkNuts/Core/Window/WindowProperties.h>
#include <VkNuts/Core/Window/WindowCallbackRegistry.h>

namespace nuts {

    struct Window : private IWindow {
        using EventCallbackSignature = std::function< void(Event&) >;
        ~Window();

        // CreateWindow is reserved in Windows "WinUser.h" so I used Create
        // Create only initializes a new Window object, call Init and Finalize to open and close the window
        static UniqueRef< Window > Create();

        const GLFWwindow*  getHandle() const noexcept;
        GLFWwindow*        getHandle() noexcept;
        const NativeHandle getNativeHandle() const noexcept;
        NativeHandle       getNativeHandle() noexcept;

        // Get and set window props
        std::uint32_t GetWidth() const noexcept;
        std::uint32_t GetHeight() const noexcept;
        const char*   GetTitle() const noexcept;
        bool          HasVSync() const noexcept;
        void          SetEventCallback(EventCallbackSignature&& callback) noexcept;
        void          SetVSync(bool vsync) noexcept;
        /// <summary>
        /// Uses mRegistry to query "callbackLib" for callbacks and assign them to the mWindow
        /// </summary>
        /// <param name="callbackLib"></param>
        /// <returns></returns>
        bool UpdateWindowCallbacks(const char* callbackLib, const char* alias) noexcept;

        // Virtual functions to be implemented by platform-specific windows
        // Prob not needed
        // virtual void OnUpdate() = 0;

        /// <summary>
        /// Use default callbackLib location "vknuts-windowcallback. dll/so"
        /// Also stores "vknuts-windowcallback. dll/so" in mRegistry
        /// alias: vknuts-windowcallback
        /// </summary>
        /// <param name="windowProperties"></param>
        /// <returns></returns>
        bool Init(const WindowProperties& windowProperties) noexcept;
        bool Init(const WindowProperties& windowProperties, const char* callbackLib, const char* alias) noexcept;
        bool Finalize() noexcept;

      protected:
        Window();
        DELETE_COPY_CLASS(Window)
        DELETE_MOVE_CLASS(Window)

        GLFWwindow*      mWindow;
        WindowProperties mWindowProperties;

        static std::atomic_bool       isGLFWInitialized;
        static std::atomic_uint32_t   nWindowsInitialized;
        static std::mutex             mMutex;
        static WindowCallbackRegistry mRegistry;

        static bool LoadAttachment(const char* callbackLib, const char* alias) noexcept;
        static bool UnloadAttachment(const char* alias) noexcept;
    };
} // namespace nuts