//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/glm.hpp>

#include "tools/Logger.h"

#include <iostream>

int main() {

	GLFWwindow* window = glfwCreateWindow(640, 480, "POG", NULL, NULL);
	glfwMakeContextCurrent(window);
	Rave::Logger logger = Rave::Logger();
	logger.init();
	glm::vec3 testingVec1(1.0, 0.0, 0.0);
	glm::vec3 testingVec2(0.0, 1.0, 0.0);
	glm::vec3 sol = glm::cross(testingVec1, testingVec2);
	void(*displayVec)(glm::vec3) = [](glm::vec3 thing)-> void {std::cout << "[" << thing.x << ", " << thing.y << ", " << thing.z << "]";};
	displayVec(testingVec1); std::cout << " x "; displayVec(testingVec2); std::cout << " = "; displayVec(sol); std::cout << "\n";
	RAVE_CORE_TRACE("Trace\t[{}, {}, {}] x [{}, {}, {}] = [{}, {}, {}]", testingVec1.x, testingVec1.y, testingVec1.z, testingVec2.x, testingVec2.y, testingVec2.z, sol.x, sol.y, sol.z);
	RAVE_CORE_LOG("Log\t[{}, {}, {}] x [{}, {}, {}] = [{}, {}, {}]", testingVec1.x, testingVec1.y, testingVec1.z, testingVec2.x, testingVec2.y, testingVec2.z, sol.x, sol.y, sol.z);
	RAVE_CORE_DEBUG("Debug\t[{}, {}, {}] x [{}, {}, {}] = [{}, {}, {}]", testingVec1.x, testingVec1.y, testingVec1.z, testingVec2.x, testingVec2.y, testingVec2.z, sol.x, sol.y, sol.z);
	RAVE_CORE_WARN("Warn\t[{}, {}, {}] x [{}, {}, {}] = [{}, {}, {}]", testingVec1.x, testingVec1.y, testingVec1.z, testingVec2.x, testingVec2.y, testingVec2.z, sol.x, sol.y, sol.z);
	RAVE_CORE_ERROR("Error\t[{}, {}, {}] x [{}, {}, {}] = [{}, {}, {}]", testingVec1.x, testingVec1.y, testingVec1.z, testingVec2.x, testingVec2.y, testingVec2.z, sol.x, sol.y, sol.z);
	RAVE_CORE_FATAL("Fatal\t[{}, {}, {}] x [{}, {}, {}] = [{}, {}, {}]", testingVec1.x, testingVec1.y, testingVec1.z, testingVec2.x, testingVec2.y, testingVec2.z, sol.x, sol.y, sol.z);
	
	system("pause"); //this line will crash because FATAL will DebugBreak()
	return 4;
}