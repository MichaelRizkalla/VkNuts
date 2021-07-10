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
        static UniqueRef< Window > create();

        const GLFWwindow*  getHandle() const noexcept;
        GLFWwindow*        getHandle() noexcept;
        const NativeHandle getNativeHandle() const noexcept;
        NativeHandle       getNativeHandle() noexcept;

        // Get and set window props
        std::uint32_t getWidth() const noexcept;
        std::uint32_t getHeight() const noexcept;
        const char*   getTitle() const noexcept;
        bool          hasVSync() const noexcept;
        void          setEventCallback(EventCallbackSignature&& callback) noexcept;
        void          setVSync(bool vsync) noexcept;
        /// <summary>
        /// Uses mRegistry to query "callbackLib" for callbacks and assign them to the mWindow
        /// </summary>
        /// <param name="callbackLib"></param>
        /// <returns></returns>
        bool updateWindowCallbacks(const char* callbackLib, const char* alias) noexcept;

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
        bool init(const WindowProperties& windowProperties) noexcept;
        bool init(const WindowProperties& windowProperties, const char* callbackLib, const char* alias) noexcept;
        bool finalize() noexcept;

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

        static bool loadAttachment(const char* callbackLib, const char* alias) noexcept;
        static bool unloadAttachment(const char* alias) noexcept;
    };
} // namespace nuts