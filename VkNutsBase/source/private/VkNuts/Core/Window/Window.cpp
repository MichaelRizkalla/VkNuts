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

    Window::~Window() { 
        finalize(); 
    };

    UniqueRef< Window > Window::create() {
        struct WindowMaker : public Window {
            WindowMaker()  = default;
            ~WindowMaker() = default;
        };

        std::pmr::polymorphic_allocator< std::byte > alloc {};

        return allocate_unique< Window, WindowMaker >(alloc);
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
    std::uint32_t Window::getWidth() const noexcept { return mWindowProperties.Width; }

    std::uint32_t Window::getHeight() const noexcept { return mWindowProperties.Height; }

    const char* Window::getTitle() const noexcept { return mWindowProperties.Title; }

    bool Window::hasVSync() const noexcept { return mWindowProperties.VSync; }

    void Window::setEventCallback(EventCallbackSignature&& callback) noexcept { mWindowProperties.EventCallback = callback; }

    void Window::setVSync(bool vsync) noexcept { mWindowProperties.VSync = vsync; }

    bool Window::updateWindowCallbacks(const char* callbackLib, const char* alias) noexcept {

        auto attachment = mRegistry.getAttachment(alias);
        if (!attachment) {
            mRegistry.attachAttachment(alias, callbackLib);
            attachment = mRegistry.getAttachment(alias);
        }

        try {
            auto data = attachment->getData();

            glfwSetWindowSizeCallback(mWindow, data.windowResizeFunc);
            glfwSetWindowCloseCallback(mWindow, data.windowCloseFunc);
            glfwSetKeyCallback(mWindow, data.keyIOFunc);
            glfwSetCharCallback(mWindow, data.charIOFunc);
            glfwSetMouseButtonCallback(mWindow, data.mouseButtonFunc);
            glfwSetScrollCallback(mWindow, data.mouseScrollFunc);
            glfwSetCursorPosCallback(mWindow, data.cursorPositionFunc);

            return true;
        } catch (...) { return false; }
    }

    bool Window::init(const WindowProperties& windowProperties) noexcept {

        return init(windowProperties,
#if defined(NUTS_OS_WINDOWS)
                    "vknuts-windowcallback.dll"
#elif defined(NUTS_OS_LINUX)
                    "vknuts-windowcallback.so"
#else
    #error "OS is not yet supported!"
#endif
                    ,
                    "vknuts-windowcallback");
    }

    bool Window::init(const WindowProperties& windowProperties, const char* callbackLib, const char* alias) noexcept {
        mWindowProperties = windowProperties;
        mRegistry.init(nullptr);

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
        return updateWindowCallbacks(callbackLib, alias);
    }

    bool Window::finalize() noexcept {
        glfwDestroyWindow(mWindow);

        nWindowsInitialized--;

        if (!nWindowsInitialized.load()) { glfwTerminate(); }

        mRegistry.finalize();
        return true;
    }

    bool Window::loadAttachment(const char* callbackLib, const char* alias) noexcept { return mRegistry.attachAttachment(alias, callbackLib); }

    bool Window::unloadAttachment(const char* alias) noexcept { return mRegistry.detachAttachment(alias); }

} // namespace nuts