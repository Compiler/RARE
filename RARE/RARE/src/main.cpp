#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

int main() {

	GLFWwindow* window = glfwCreateWindow(640, 480, "POG", NULL, NULL);
	glfwMakeContextCurrent(window);
	while (true);
	return 4;
}