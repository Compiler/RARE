#pragma once
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Tools/Logger.h>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <string>
#include <Rendering/FileTypes/ShaderCompilation.h>
#include <Tools/GLFWCallbacks.h>


#define RARE_INTERNAL(x) "resources/" x
#define RARE_INTERNAL_SHADER(x) RARE_INTERNAL("shaders/") x
#define FPS_COUNTER_LOGGED 0
namespace Rare {
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct VertexData {
		glm::vec3 position;
		glm::vec3 color;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VertexData);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(VertexData, position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(VertexData, color);

			return attributeDescriptions;
		}
	};
	class RareCore {

	private:

		
		const std::vector<VertexData> _vertices = {
							{{ 1.0f, -1.0f, 0.0f}, {0.0, 0.0, 1.0}},
							{{-1.0f,  1.0f, 0.0f}, {0.0, 1.0, 0.0}},
							{{-1.0f, -1.0f, 0.0f}, {1.0, 0.0, 0.0}},
							{{ 1.0f,  1.0f, 0.0f}, {1.0, 0.0, 0.0}},
							{{-1.0f,  1.0f, 0.0f}, {0.0, 1.0, 0.0}},
							{{ 1.0f, -1.0f, 0.0f}, {0.0, 0.0, 1.0}}
						};
		

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
		const int _WIDTH = 640, _HEIGHT = 480;
		const int _MAX_FRAMES_IN_FLIGHT = 2;
		size_t _currentFrame = 0;

		//vk setup core
		VkInstance _vkInstance;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		VkDevice _logicalDevice;
		VkDebugUtilsMessengerEXT _debugMessenger;
		VkQueue _graphicsQueue;
		VkQueue _presentationQueue;		
		std::vector<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; //these are the absolute bare minimum required extensions to be considered suitable

		//wsi surface integration
		VkSurfaceKHR _surface;

		//swap chain data
		VkSwapchainKHR _swapChain;
		std::vector<VkImage> _swapChainImages;
		VkFormat _swapChainImageFormat;
		VkExtent2D _swapChainExtent;
		std::vector<VkImageView> _swapChainImageViews;

		//graphics pipeline
		VkPipeline _graphicsPipeline;
		VkRenderPass _renderPass;
		VkPipelineLayout _pipelineLayout;

		//frame buffer
		std::vector<VkFramebuffer> _swapChainFramebuffers;

		//command buffers
		VkCommandPool _commandPool;
		std::vector<VkCommandBuffer> _commandBuffers;

		//Semaphores (denoted with s_)
		std::vector<VkSemaphore> _s_imageAvailable, _s_renderFinished;

		//Fences (denoted with a f_)
		std::vector<VkFence> _f_inFlight, _f_imagesInFlight;

		//Vertex Buffer object
		VkBuffer _vertexBuffer;
		VkDeviceMemory _vertexBufferMemory;

		void _createVkInstance();
		void _createSurface();
		void _createLogicalDevice();
		void _createSwapChain();
		void _createImageViews();
		void _createRenderPass();
		void _createGraphicsPipeline();
		void _createFramebuffers();
		void _createCommandPool();
		void _createCommandBuffers();
		void _createSynchronizationObjects();
		void _pickPhysicalDevice();
		void _setupDebugMessenger();
		void _recreateSwapChain();
		void _cleanupSwapChain();
		void _createVertexBuffer();
		void _createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void _copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);


		void _populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		

		uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		bool _isDeviceSuitable(VkPhysicalDevice device);//TODO: move to seperate factory class or something
		bool _checkValidationLayerSupport();
		QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);
		VkShaderModule _createShaderModule(const std::vector<uint32_t>& code);
		std::vector<const char*> _getRequiredExtensions();
		SwapChainSupportDetails _querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	public:

		bool framebufferResized = false;

		RareCore();
		RareCore(const char* windowName);
		RareCore(const char* windowName, const std::vector<const char*> validationLayers);
		void init();
		void update();
		void render();
		void waitIdle();
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
				RARE_FATAL("\nVALIDATION FATAL\n:{}:{}\t{}\n", __FILENAME__, __LINE__, pCallbackData->pMessage)
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
				RARE_ERROR("\nVALIDATION ERROR\n:{}:{}\t{}\n", __FILENAME__, __LINE__, pCallbackData->pMessage);
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
				RARE_WARN("\nVALIDATION WARN\N:{}:{}\t{}\n", __FILENAME__, __LINE__, pCallbackData->pMessage);
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
				RARE_LOG("\nVALIDATION LOG\n:{}:{}\t{}\n", __FILENAME__, __LINE__, pCallbackData->pMessage);
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
