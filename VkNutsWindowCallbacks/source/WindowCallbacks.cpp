#include <WindowCallbacks.h>

#include <VkNuts/Core/Event/IOEvents.h>
#include <VkNuts/Core/Event/WindowEvents.h>
#include <VkNuts/Core/Window/WindowProperties.h>
#include <Platform/Generic/MouseCode.h>
#include <Platform/Generic/KeyCode.h>

const char* GetUsage() {
    return "WindowCallbackAttachment";
}

void OnChar(GLFWwindow* window, unsigned int keyCode) {
    nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

    nuts::KeyCharacterEvent mEvent { static_cast< nuts::KeyCode >(keyCode) };
    data.EventCallback(mEvent);
}

void OnCursorPosition(GLFWwindow* window, double xPos, double yPos) {
    nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

    nuts::MouseMovedEvent mEvent { xPos, yPos };
    data.EventCallback(mEvent);
}

void OnKey(GLFWwindow* window, int key, int scanCode, int action, int mods) {
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

void OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
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

void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset) {
    nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

    nuts::MouseScrollTriggeredEvent mEvent { xOffset, yOffset };
    data.EventCallback(mEvent);
}

void OnWindowClose(GLFWwindow* window) {
    nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));

    nuts::WindowClosedEvent mEvent {};
    data.EventCallback(mEvent);
}

void OnWindowReSize(GLFWwindow* window, int width, int height) {
    nuts::WindowProperties& data = *reinterpret_cast< nuts::WindowProperties* >(glfwGetWindowUserPointer(window));
    data.Width                   = width;
    data.Height                  = height;

    nuts::WindowResizedEvent mEvent { width, height };
    data.EventCallback(mEvent);
}