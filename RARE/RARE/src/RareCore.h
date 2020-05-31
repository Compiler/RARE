#pragma once
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <tools/Logger.h>
#include <map>
#include <optional>


namespace Rare {

	class RareCore {

	private:

	#ifdef NDEBUG
		const bool _enableValidationLayers = false;
	#else
		const bool _enableValidationLayers = true;
	#endif

		struct QueueFamilyIndices;
		bool _coreShouldClose;

		//TODO: Abstract 'window' into its own class
		GLFWwindow* _windowRef;
		const char* _windowRefName;
		const std::vector<const char*> _validationLayers;

		VkInstance _vkInstance;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		VkDevice _logicalDevice;
		VkDebugUtilsMessengerEXT _debugMessenger;
		VkQueue _graphicsQueue;

		bool _isDeviceSuitable(VkPhysicalDevice device);//TODO: move to seperate factory class or something
		void _createVkInstance();
		void _pickPhysicalDevice();
		void _createLogicalDevice();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		void _setupDebugMessenger();
		bool _checkValidationLayerSupport();
		std::vector<const char*> _getRequiredExtensions();
		void _populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		
	public:
		RareCore();
		RareCore(const char* windowName);
		RareCore(const char* windowName, const std::vector<const char*> validationLayers);
		void init();
		void update();
		void render();
		void dispose();

		inline bool shouldClose() const { return _coreShouldClose; }
		

	//Static and Const
	private:



















		static VKAPI_ATTR VkBool32 VKAPI_CALL _debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {
			if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
				RARE_FATAL("{}:{}\t{}", __FILENAME__, __LINE__, pCallbackData->pMessage);
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
				RARE_ERROR("{}:{}\t{}", __FILENAME__, __LINE__, pCallbackData->pMessage);
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
				RARE_WARN("{}:{}\t{}", __FILENAME__, __LINE__, pCallbackData->pMessage);
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
				RARE_LOG("{}:{}\t{}", __FILENAME__, __LINE__, pCallbackData->pMessage);
			return VK_FALSE;
		}
		static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {

			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}
		static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator) {

			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(instance, debugMessenger, pAllocator);
			}
		}
	};

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);
	

}
