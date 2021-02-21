#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Window/Window.h>
#include <VkNuts/Core/Event/WindowEvents.h>
#include <VkNuts/Core/Event/IOEvents.h>

#if defined(NUTS_OS_WINDOWS)
    #define GLFW_EXPOSE_NATIVE_WIN32
#else
    #error "Platform not yet supported!"
#endif
#include <GLFW/glfw3native.h>

namespace nuts {

    std::atomic_bool       Window::isGLFWInitialized   = false;
    std::atomic_uint32_t   Window::nWindowsInitialized = 0;
    std::mutex             Window::mMutex;
    WindowCallbackRegistry Window::mRegistry = WindowCallbackRegistry {};

    Window::Window() : mWindow(nullptr) {}

    Window::~Window() = default;

    UniqueRef< Window > Window::Create() {
        struct WindowMaker : public Window {
            WindowMaker()  = default;
            ~WindowMaker() = default;
        };

        return std::make_unique< WindowMaker >();
    }

    const GLFWwindow* Window::getHandle() const noexcept { return reinterpret_cast< const GLFWwindow* >(mWindow); }

    GLFWwindow* Window::getHandle() noexcept { return mWindow; }

    const Window::NativeHandle Window::getNativeHandle() const noexcept {
#if defined(NUTS_OS_WINDOWS)
        return glfwGetWin32Window(mWindow);
#else
    #error "Platform not yet supported!"
#endif
    }

    Window::NativeHandle Window::getNativeHandle() noexcept { return const_cast< NativeHandle >(reinterpret_cast< const Window* >(this)->getNativeHandle()); }

    // Get and set window props
    std::uint32_t Window::GetWidth() const noexcept { return mWindowProperties.Width; }

    std::uint32_t Window::GetHeight() const noexcept { return mWindowProperties.Height; }

    const char* Window::GetTitle() const noexcept { return mWindowProperties.Title; }

    bool Window::HasVSync() const noexcept { return mWindowProperties.VSync; }

    void Window::SetEventCallback(EventCallbackSignature&& callback) noexcept { mWindowProperties.EventCallback = callback; }

    void Window::SetVSync(bool vsync) noexcept { mWindowProperties.VSync = vsync; }

    bool Window::UpdateWindowCallbacks(const char* callbackLib, const char* alias) noexcept {
        if (!mRegistry.HasAttachment(alias)) { mRegistry.AttachAttachment(alias, callbackLib); }
        try {
            auto data = mRegistry.QueryAttachment(alias);

            glfwSetWindowSizeCallback(mWindow, data.WindowResizeFunc);
            glfwSetWindowCloseCallback(mWindow, data.WindowCloseFunc);
            glfwSetKeyCallback(mWindow, data.KeyIOFunc);
            glfwSetCharCallback(mWindow, data.CharIOFunc);
            glfwSetMouseButtonCallback(mWindow, data.MouseButtonFunc);
            glfwSetScrollCallback(mWindow, data.MouseScrollFunc);
            glfwSetCursorPosCallback(mWindow, data.CursorPositionFunc);

            return true;
        } catch (...) { return false; }
    }

    bool Window::Init(const WindowProperties& windowProperties) noexcept {

        return Init(windowProperties,
#if defined(NUTS_OS_WINDOWS)
                    "vknuts-windowcallback.dll"
#elif defined(NUTS_OS_LINUX)
                    "vknuts-windowcallback.so"
#else
    #error "OS is not yet supported!"
#endif
        , "vknuts-windowcallback");
    }

    bool Window::Init(const WindowProperties& windowProperties, const char* callbackLib, const char* alias) noexcept {
        mWindowProperties = windowProperties;
        mRegistry.Init(nullptr);

        mMutex.lock();
        if (!isGLFWInitialized.load()) {
            NUTS_ENGINE_INFO("Initializing glfw!");
            if (!glfwInit()) {
                NUTS_ENGINE_ERROR("Could not initialize glfw!");
                mMutex.unlock();
                return false;
            }
            isGLFWInitialized.store(true);
        }
        mMutex.unlock();
        NUTS_ENGINE_INFO("Creating glfw window!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        mWindow = glfwCreateWindow(static_cast< int >(mWindowProperties.Width), static_cast< int >(mWindowProperties.Height), mWindowProperties.Title, nullptr, nullptr);
        if (!mWindow) {
            NUTS_ENGINE_ERROR("Could not create window!");
            return false;
        }
        nWindowsInitialized++;
        glfwSetWindowUserPointer(mWindow, &mWindowProperties);


        // Set callbacks
        return UpdateWindowCallbacks(callbackLib, alias);
    }

    bool Window::Finalize() noexcept {
        glfwDestroyWindow(mWindow);

        nWindowsInitialized--;

        if (!nWindowsInitialized.load()) { glfwTerminate(); }

        return true;
    }

    bool Window::LoadAttachment(const char* callbackLib, const char* alias) noexcept { return mRegistry.AttachAttachment(alias, callbackLib); }

    bool Window::UnloadAttachment(const char* alias) noexcept { return mRegistry.DetachAttachment(alias); }

} // namespace nuts