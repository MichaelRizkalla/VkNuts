#pragma once

#include <Platform/CompilerDefines.h>
#include <Platform/PlatformDefines.h>

#include <GLFW/glfw3.h>

#ifdef __cplusplus
extern "C" {
#endif

NUTS_API const char* getUsage();

NUTS_API void onChar(GLFWwindow* window, unsigned int keyCode);

NUTS_API void onCursorPosition(GLFWwindow* window, double xPos, double yPos);

NUTS_API void onKey(GLFWwindow* window, int key, int scanCode, int action, int mods);

NUTS_API void onMouseButton(GLFWwindow* window, int button, int action, int mods);

NUTS_API void onMouseScroll(GLFWwindow* window, double xOffset, double yOffset);

NUTS_API void onWindowClose(GLFWwindow* window);

NUTS_API void onWindowReSize(GLFWwindow* window, int width, int height);

#ifdef __cplusplus
}
#endif