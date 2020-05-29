#include "RareCore.h"

namespace Rare {

	RareCore::RareCore():_windowRefName("Default Name") { _coreShouldClose = false; }
	RareCore::RareCore(const char* windowName) : _windowRefName(windowName) { _coreShouldClose = false; }

	void RareCore::init() {

		//begin logger initialization
		Rare::Logger logger = Rare::Logger();
		logger.init();
		RARE_LOG("Logger:\t Initialization complete");

		//begin glfw initialization
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		_windowRef = glfwCreateWindow(640, 480, _windowRefName, NULL, NULL);
		glfwMakeContextCurrent(_windowRef);
		RARE_LOG("GLFW:\t\t Initialization complete");

		//begin vk initialization
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		RARE_DEBUG("{} extensions supported at compile time", extensionCount);
		RARE_LOG("Vulkan:\t Initialization complete");

		RARE_LOG("Initialization complete");
	}





	void RareCore::update() {
		static double start = glfwGetTime();
		static double delta;
		glfwPollEvents();//assign this to a daemon thread and lock event manager to synch assignments
		_coreShouldClose = (delta = glfwGetTime() - start) >= 5 ? true : false;
	}
	void RareCore::render() {

	}

	void RareCore::dispose() {
		glfwDestroyWindow(_windowRef);
		glfwTerminate();
	
		RARE_LOG("{} Window closed", _windowRefName);
	}
}
