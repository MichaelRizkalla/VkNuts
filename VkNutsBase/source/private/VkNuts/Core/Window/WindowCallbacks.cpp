#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <GLFW/glfw3.h>
#include <VkNuts/Core/Log/Log.h>
#include <VkNuts/Core/Window/WindowCallbacks.h>

#include <VkNuts/Core/Event/IOEvents.h>
#include <VkNuts/Core/Event/WindowEvents.h>
#include <VkNuts/Core/Window/WindowProperties.h>
#include <Platform/Generic/MouseCode.h>
#include <Platform/Generic/KeyCode.h>

namespace nuts {

    void WindowCallbacks::onChar(GLFWwindow* window, unsigned int keyCode) {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

        nuts::KeyCharacterEvent mEvent { static_cast< nuts::KeyCode >(keyCode) };
        data.EventCallback(mEvent);
    }
    void WindowCallbacks::onCursorPosition(GLFWwindow* window, double xPos, double yPos) {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

        nuts::MouseMovedEvent mEvent { xPos, yPos };
        data.EventCallback(mEvent);
    }
    void WindowCallbacks::onKey(GLFWwindow* window, int key, int scanCode, int action, int mods) {
        (void)scanCode, mods;
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                nuts::KeyPressedEvent mEvent { static_cast< nuts::KeyCode >(key), 0 };
                data.EventCallback(mEvent);
            } break;
            case GLFW_RELEASE: {
                nuts::KeyReleasedEvent mEvent { static_cast< nuts::KeyCode >(key) };
                data.EventCallback(mEvent);
            } break;
            case GLFW_REPEAT: {
                nuts::KeyPressedEvent mEvent { static_cast< nuts::KeyCode >(key), 1 };
                data.EventCallback(mEvent);
            } break;
        }
    }
    void WindowCallbacks::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
        (void)mods;
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                nuts::MouseButtonPressedEvent mEvent { static_cast< nuts::MouseCode >(button) };
                data.EventCallback(mEvent);
            } break;
            case GLFW_RELEASE: {
                nuts::MouseButtonReleasedEvent mEvent { static_cast< nuts::MouseCode >(button) };
                data.EventCallback(mEvent);
            } break;
        }
    }
    void WindowCallbacks::onMouseScroll(GLFWwindow* window, double xOffset, double yOffset) {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

        nuts::MouseScrollTriggeredEvent mEvent { xOffset, yOffset };
        data.EventCallback(mEvent);
    }
    void WindowCallbacks::onWindowClose(GLFWwindow* window) {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

        nuts::WindowClosedEvent mEvent {};
        data.EventCallback(mEvent);
    }
    void WindowCallbacks::onWindowReSize(GLFWwindow* window, int width, int height) {
        nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));
        data.Width                   = width;
        data.Height                  = height;

        nuts::WindowResizedEvent mEvent { width, height };
        data.EventCallback(mEvent);
    }

    WindowCallbacks::attachment_data WindowCallbacks::getData() noexcept {
        attachment_data data {};

        data.charIOFunc         = reinterpret_cast< attachment_data::CharIOSignature >(WindowCallbacks::onChar);
        data.cursorPositionFunc = reinterpret_cast< attachment_data::CursorPositionSignature >(WindowCallbacks::onCursorPosition);
        data.keyIOFunc          = reinterpret_cast< attachment_data::KeyIOSignature >(WindowCallbacks::onKey);
        data.mouseButtonFunc    = reinterpret_cast< attachment_data::MouseButtonSignature >(WindowCallbacks::onMouseButton);
        data.mouseScrollFunc    = reinterpret_cast< attachment_data::MouseScrollSignature >(WindowCallbacks::onMouseScroll);
        data.windowCloseFunc    = reinterpret_cast< attachment_data::WindowCloseSignature >(WindowCallbacks::onWindowClose);
        data.windowResizeFunc   = reinterpret_cast< attachment_data::WindowReSizeSignature >(WindowCallbacks::onWindowReSize);

        return data;
    }

} // namespace nuts
