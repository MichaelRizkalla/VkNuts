#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <GLFW/glfw3.h>
#include <Utilities/Utilities.hpp>
#include <Platform/Generic/IWindow.h>
#include <VkNuts/Core/Event/Event.h>
#include <VkNuts/Core/Window/WindowProperties.h>

namespace nuts {

    struct Window : public IWindow {
        using EventCallbackSignature = std::function< void(Event&) >;
        ~Window();

        // CreateWindow is reserved in Windows "WinUser.h" so I used Create
        // Create only initializes a new Window object, call Init and Finalize to open and close the window
        [[nodiscard]] static UniqueRef< Window > create();

        [[nodiscard]] const GLFWwindow*  getHandle() const noexcept;
        [[nodiscard]] GLFWwindow*        getHandle() noexcept;
        [[nodiscard]] const NativeHandle getNativeHandle() const noexcept;
        [[nodiscard]] NativeHandle       getNativeHandle() noexcept;

        // Get and set window props
        [[nodiscard]] std::uint32_t getWidth() const noexcept;
        [[nodiscard]] std::uint32_t getHeight() const noexcept;

        /// <summary>
        /// Returns std::pair of Width and Height
        /// </summary>
        [[nodiscard]] std::pair< std::uint32_t, std::uint32_t > getDimensions() const noexcept;
        [[nodiscard]] const char*                               getTitle() const noexcept;
        [[nodiscard]] bool                                      hasVSync() const noexcept;
        void                                                    setEventCallback(EventCallbackSignature&& callback) noexcept;
        void                                                    setVSync(bool vsync) noexcept;
        /// <summary>
        /// Uses mRegistry to query "callbackLib" for callbacks and assign them to the mWindow
        /// </summary>
        /// <param name="callbackLib"></param>
        /// <returns></returns>
        [[nodiscard]] bool updateWindowCallbacks() noexcept;

        // Virtual functions to be implemented by platform-specific windows
        // Prob not needed
        void onUpdate();

        /// <summary>
        /// Use default callbackLib location "vknuts-windowcallback. dll/so"
        /// Also stores "vknuts-windowcallback. dll/so" in mRegistry
        /// alias: default-callback
        /// </summary>
        /// <param name="windowProperties"></param>
        /// <returns></returns>
        [[nodiscard]] bool init(const WindowProperties& windowProperties) noexcept;
        void               finalize() noexcept;

      protected:
        Window();
        DELETE_COPY_CLASS(Window)
        DELETE_MOVE_CLASS(Window)

        GLFWwindow*      mWindow;
        WindowProperties mWindowProperties;

        static std::atomic_bool     isGLFWInitialized;
        static std::atomic_uint32_t nWindowsInitialized;
        static std::mutex           mMutex;
    };
} // namespace nuts
