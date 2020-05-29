#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/glm.hpp>

#include "tools/Logger.h"

#include <iostream>

GLFWwindow* window;

void initLogger();
void initGLFW();
void initVK();
void init();
void update();
void render();

int main() {
	
	
	

	init();


	while (!glfwWindowShouldClose(window)) {
		update(); render();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	
	system("pause"); //this line will crash because FATAL will DebugBreak()
	return 4;
}




//log init subrt
void initLogger() {
	Rave::Logger logger = Rave::Logger();
	logger.init();
	RAVE_CORE_LOG("Logger:\t Initialization complete");
}
//glfw init subrt
void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(640, 480, "RAVE", NULL, NULL);
	glfwMakeContextCurrent(window);
	RAVE_CORE_LOG("GLFW:\t\t Initialization complete");
}

//vk init subrt
void initVK() {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	RAVE_CORE_DEBUG("{} extensions supported at compile time", extensionCount);
	RAVE_CORE_LOG("Vulkan:\t Initialization complete");
}


void init() {
	initLogger();
	initGLFW();
	initVK();
}
void update() {
	glfwPollEvents();

}
void render() {

}