#pragma once

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <tools/Logger.h>

#include <map>

namespace Rare {

	class RareCore {
#ifdef NDEBUG
		const bool _enableValidationLayers = false;
#else
		const bool _enableValidationLayers = true;
#endif

	private:

		const std::vector<const char*> _validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		bool _coreShouldClose;

		//TODO: Abstract 'window' into its own class
		GLFWwindow* _windowRef;
		const char* _windowRefName;

		VkInstance _vkInstance;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT debugMessenger;

		bool _isDeviceSuitable(VkPhysicalDevice device);//TODO: move to seperate factory class or something
		void _createVkInstance();
		void _pickPhysicalDevice();
		void _setupDebugMessenger();
		bool _checkValidationLayerSupport();
		static VKAPI_ATTR VkBool32 VKAPI_CALL _debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {
			if(messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
				RARE_FATAL(pCallbackData->pMessage);
			if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
				RARE_ERROR(pCallbackData->pMessage);
			if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
				RARE_WARN(pCallbackData->pMessage);
			if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
				RARE_LOG(pCallbackData->pMessage);
			return VK_FALSE;
		}
		std::vector<const char*> _getRequiredExtensions();
	public:
		RareCore();
		RareCore(const char* windowName);
		void init();
		void update();
		void render();
		void dispose();

		inline bool shouldClose() const { return _coreShouldClose; }


	};
	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

}
