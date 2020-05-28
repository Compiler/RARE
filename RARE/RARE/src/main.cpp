//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/glm.hpp>

#include <iostream>

int main() {

	GLFWwindow* window = glfwCreateWindow(640, 480, "POG", NULL, NULL);
	glfwMakeContextCurrent(window);

	glm::vec3 testingVec1(1.0, 0.0, 0.0);
	glm::vec3 testingVec2(0.0, 1.0, 0.0);
	glm::vec3 sol = glm::cross(testingVec1, testingVec2);
	void(*displayVec)(glm::vec3) = [](glm::vec3 thing)-> void {std::cout << "[" << thing.x << ", " << thing.y << ", " << thing.z << "]";};
	displayVec(testingVec1); std::cout << " x "; displayVec(testingVec2); std::cout << " = "; displayVec(sol); std::cout << "\n";
	system("pause");
	return 4;
}