#pragma once

#include <Platform/CompilerDefines.h>
#include <Platform/PlatformDefines.h>

#include <GLFW/glfw3.h>

#ifdef __cplusplus
extern "C" {
#endif

NUTS_API const char* GetUsage();

NUTS_API void OnChar(GLFWwindow* window, unsigned int keyCode);

NUTS_API void OnCursorPosition(GLFWwindow* window, double xPos, double yPos);

NUTS_API void OnKey(GLFWwindow* window, int key, int scanCode, int action, int mods);

NUTS_API void OnMouseButton(GLFWwindow* window, int button, int action, int mods);

NUTS_API void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset);

NUTS_API void OnWindowClose(GLFWwindow* window);

NUTS_API void OnWindowReSize(GLFWwindow* window, int width, int height);

#ifdef __cplusplus
}
#endif