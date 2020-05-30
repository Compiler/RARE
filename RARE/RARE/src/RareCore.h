#pragma once

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <tools/Logger.h>

#include <map>

namespace Rare {
	class RareCore {

	private:
		bool _coreShouldClose;

		//TODO: Abstract 'window' into its own class
		GLFWwindow* _windowRef;
		const char* _windowRefName;

		VkInstance _vkInstance;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;

		bool _isDeviceSuitable(VkPhysicalDevice device);//TODO: move to seperate factory class or something
		void _createVkInstance();
		void _pickPhysicalDevice();
	public:
		RareCore();
		RareCore(const char* windowName);
		void init();
		void update();
		void render();
		void dispose();

		inline bool shouldClose() const { return _coreShouldClose; }


	};

}
