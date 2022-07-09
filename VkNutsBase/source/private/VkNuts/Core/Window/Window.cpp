#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Window/Window.h>
#include <VkNuts/Core/Event/WindowEvents.h>
#include <VkNuts/Core/Event/IOEvents.h>
#include <VkNuts/Core/Window/WindowCallbacks.h>

#if defined(NUTS_OS_WINDOWS)
    #define GLFW_EXPOSE_NATIVE_WIN32
#else
    #error "Platform not yet supported!"
#endif
#include <GLFW/glfw3native.h>

namespace nuts {

    std::atomic_bool     Window::isGLFWInitialized   = false;
    std::atomic_uint32_t Window::nWindowsInitialized = 0;
    std::mutex           Window::mMutex;

    Window::Window() : mWindow(nullptr) {
    }

    Window::~Window() {
        finalize();
    };

    UniqueRef< Window > Window::create() {
        struct WindowMaker : public Window {
            WindowMaker()  = default;
            ~WindowMaker() = default;
        };

        return UniqueRef< Window >(new WindowMaker);
    }

    const GLFWwindow* Window::getHandle() const noexcept {
        return reinterpret_cast< const GLFWwindow* >(mWindow);
    }

    GLFWwindow* Window::getHandle() noexcept {
        return mWindow;
    }

    const Window::NativeHandle Window::getNativeHandle() const noexcept {
#if defined(NUTS_OS_WINDOWS)
        return glfwGetWin32Window(mWindow);
#else
    #error "Platform not yet supported!"
#endif
    }

    Window::NativeHandle Window::getNativeHandle() noexcept {
        return const_cast< NativeHandle >(reinterpret_cast< const Window* >(this)->getNativeHandle());
    }

    // Get and set window props
    std::uint32_t Window::getWidth() const noexcept {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(mWindow));
        return static_cast< std::uint32_t >(data.Width);
    }

    std::uint32_t Window::getHeight() const noexcept {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(mWindow));
        return static_cast< std::uint32_t >(data.Height);
    }

    std::pair< std::uint32_t, std::uint32_t > Window::getDimensions() const noexcept {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(mWindow));
        return std::make_pair(static_cast< std::uint32_t >(data.Width), static_cast< std::uint32_t >(data.Height));
    }

    const char* Window::getTitle() const noexcept {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(mWindow));
        return data.Title;
    }

    bool Window::hasVSync() const noexcept {
        return mWindowProperties.VSync;
    }

    void Window::setEventCallback(EventCallbackSignature&& callback) noexcept {
        mWindowProperties.EventCallback = callback;
    }

    void Window::setVSync(bool vsync) noexcept {
        if (vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        mWindowProperties.VSync = vsync;
    }

    bool Window::updateWindowCallbacks() noexcept {

        auto data = WindowCallbacks::getData();

        glfwSetWindowSizeCallback(mWindow, data.windowResizeFunc);
        glfwSetWindowCloseCallback(mWindow, data.windowCloseFunc);
        glfwSetKeyCallback(mWindow, data.keyIOFunc);
        glfwSetCharCallback(mWindow, data.charIOFunc);
        glfwSetMouseButtonCallback(mWindow, data.mouseButtonFunc);
        glfwSetScrollCallback(mWindow, data.mouseScrollFunc);
        glfwSetCursorPosCallback(mWindow, data.cursorPositionFunc);

        return true;
    }

    void Window::onUpdate() {
        glfwPollEvents();
    }

    bool Window::init(const WindowProperties& windowProperties) noexcept {
        mWindowProperties = windowProperties;

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
        return updateWindowCallbacks();
    }

    void Window::finalize() noexcept {
        glfwDestroyWindow(mWindow);

        nWindowsInitialized--;

        if (!nWindowsInitialized.load()) {
            glfwTerminate();
        }
    }

} // namespace nuts
