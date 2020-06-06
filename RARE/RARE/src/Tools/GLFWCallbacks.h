#pragma once
#include <GLFW/glfw3.h>
#include <RareCore.h>
class GLFWCallbacks {

public:
	GLFWCallbacks() = delete;
	GLFWCallbacks(const GLFWCallbacks&) = delete;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

