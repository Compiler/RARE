#include "GLFWCallbacks.h"


void GLFWCallbacks::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	Rare::RareCore* appRef = reinterpret_cast<Rare::RareCore*>(glfwGetWindowUserPointer(window));
	appRef->framebufferResized = true;
}
