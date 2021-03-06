#pragma once
#include "RareCommon.h"
#include "RareStructures.h"
#include "Rendering/Data/BufferFactory.h"
#include "Rendering/Commands/CmdBufferFactory.h"
#include <Tools/Logging/Logger.h>
#include "Tools/GLFWCallbacks.h"
#include <Rendering/FileTypes/ShaderCompilation.h>
#include <Tools/FileLoaders/FileLoaderFactory.h>



#define FPS_COUNTER_LOGGED 1
namespace Rare {
	
	class RareCore {

	private:

		static constexpr float a = 0.75f;
		std::vector<VertexData> _vertices;/* = {
							{{ a, -a, 0.0f}, {0.0, 0.0, 1.0}, {1.0f, 0.0f}},	//0
							{{-a,  a, 0.0f}, {0.0, 1.0, 0.0}, {0.0f, 1.0f}},	//1
							{{-a, -a, 0.0f}, {1.0, 0.0, 0.0}, {0.0f, 0.0f}},	//2
							{{ a,  a, 0.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f}},	//3

							{{ a, -a, -0.5f }, {0.0, 0.0, 1.0}, {1.0f, 0.0f}},	//4
							{{-a,  a, -0.5f}, {0.0, 1.0, 0.0}, {0.0f, 1.0f}},	//5
							{{-a, -a, -0.5f}, {1.0, 0.0, 0.0}, {0.0f, 0.0f}},	//6
							{{ a,  a, -0.5f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f}}		//7
		};*/
		static constexpr int offset = 4;
		std::vector<uint32_t> _indices;/* = { 0,1,2,3,1,0,	4, 5, 6, 7, 5, 4 };*/


	#ifdef NDEBUG
		const bool _enableValidationLayers = false;
	#else
		const bool _enableValidationLayers = true;
	#endif

		struct QueueFamilyIndices;
		bool _coreShouldClose;
		float deltaTime;
		//TODO: Abstract 'window' into its own class
		GLFWwindow* _windowRef;
		const char* _windowRefName;
		const std::vector<const char*> _validationLayers;
		const int _WIDTH = 640, _HEIGHT = 480;
		const int _MAX_FRAMES_IN_FLIGHT = 2;
		size_t _currentFrame = 0;
		const std::string _MODEL_PATH = RARE_INTERNAL_MODEL("viking_room.obj");
		const std::string _TEXTURE_PATH = RARE_INTERNAL_TEXTURE("viking_room.png");

		//vk setup core
		VulkanContext _vkContext;
		VkQueue _presentationQueue;
		VkDebugUtilsMessengerEXT _debugMessenger;
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
		VkDescriptorSetLayout _descriptorSetLayout;
		VkPipelineLayout _pipelineLayout;

		//frame buffer
		std::vector<VkFramebuffer> _swapChainFramebuffers;

		//command buffers
		std::vector<VkCommandBuffer> _commandBuffers;

		//Semaphores (denoted with s_)
		std::vector<VkSemaphore> _s_imageAvailable, _s_renderFinished;

		//Fences (denoted with a f_)
		std::vector<VkFence> _f_inFlight, _f_imagesInFlight;

		//Buffers
		VkBuffer _vertexBuffer;
		VkDeviceMemory _vertexBufferMemory;
		VkBuffer _indexBuffer;
		VkDeviceMemory _indexBufferMemory;

		//ubo data
		std::vector<VkBuffer> _uniformBuffers;
		std::vector<VkDeviceMemory> _uniformBuffersMemory;
		VkDescriptorPool _descriptorPool;
		std::vector<VkDescriptorSet> _descriptorSets;

		//texture data
		uint32_t _mipLevels;
		VkImage _textureImage;
		VkDeviceMemory _textureImageMemory;
		VkImageView _textureImageView;
		VkSampler _textureSampler;

		//depth buffering
		VkImage _depthImage;
		VkDeviceMemory _depthImageMemory;
		VkImageView _depthImageView;

		//sample count per physical device
		VkSampleCountFlagBits _msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		VkImage _colorImage;
		VkDeviceMemory _colorImageMemory;
		VkImageView _colorImageView;


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
		void _createDescriptorSetLayout();
		void _createUniformBuffers();
		void _createDescriptorPool();
		void _createDescriptorSets();
		void _createTextureImage();
		void _createTextureImageView();
		void _createTextureSampler();
		void _createDepthResources();
		void _loadModel();
		void _generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		void _createColorResources();

		VkImageView _createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		void _copyBufferToImage(VkBuffer buff, VkImage img, uint32_t width, uint32_t height);
		void _transitionImageLayout(VkImage img, VkFormat fmt, VkImageLayout olay, VkImageLayout nlay, uint32_t mipLevels);
		void _createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, uint32_t mipLevels, VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT);
		

		VkFormat _findDepthFormat() {
			return _findSupportedBitFormats(
				{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
				VK_IMAGE_TILING_OPTIMAL,
				VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
			);
		}
		VkFormat  _findSupportedBitFormats(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		bool _hasStencilComponent(VkFormat format) {
			return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
		}

		VkSampleCountFlagBits getMaxUsableSampleCount() {
			VkPhysicalDeviceProperties physicalDeviceProperties;
			vkGetPhysicalDeviceProperties(_vkContext.physicalDevice, &physicalDeviceProperties);

			VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
			if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
			if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
			if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
			if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
			if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
			if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

			return VK_SAMPLE_COUNT_1_BIT;
		}

		void _updateUniformBuffer(uint32_t imageIndex);
		void _populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

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
			if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
				RARE_FATAL("\nVALIDATION FATAL\n:{}:{}\t{}\n", __FILENAME__, __LINE__, pCallbackData->pMessage);
			}
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
				RARE_ERROR("\nVALIDATION ERROR\n:{}:{}\t{}\n", __FILENAME__, __LINE__, pCallbackData->pMessage);
			}
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
				RARE_WARN("\nVALIDATION WARN\N:{}:{}\t{}\n", __FILENAME__, __LINE__, pCallbackData->pMessage);
			}
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
				RARE_LOG("\nVALIDATION LOG\n:{}:{}\t{}\n", __FILENAME__, __LINE__, pCallbackData->pMessage);
			}
			return VK_FALSE;
		}
		static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {

			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			} else {
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
