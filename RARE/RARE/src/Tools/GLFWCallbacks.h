#pragma once
#include <GLFW/glfw3.h>
#include <RareCore.h>
#include <Tools/Input/InputManager.h>
class GLFWCallbacks {

public:
	GLFWCallbacks() = delete;
	GLFWCallbacks(const GLFWCallbacks&) = delete;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
};

