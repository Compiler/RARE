#pragma once
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <Tools/Logger.h>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <optional>
#include <Rendering/FileTypes/ShaderCompilation.h>

namespace Rare {
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentationFamily;

		//method to show whether or not a set of needed commands have be acquired 
		bool isComplete() {
			return graphicsFamily.has_value() && presentationFamily.has_value();
		}
	};


	class GPUDevice {


	private:

		//graphics pipeline
		VkPipeline _graphicsPipeline;
		VkRenderPass _renderPass;
		VkPipelineLayout _pipelineLayout;

		//swap chain data
		VkSwapchainKHR _swapChain;
		std::vector<VkImage> _swapChainImages;
		VkFormat _swapChainImageFormat;
		VkExtent2D _swapChainExtent;
		std::vector<VkImageView> _swapChainImageViews;
		std::vector<VkFramebuffer> _swapChainFramebuffers;

		//devices
		VkPhysicalDevice _gpu = VK_NULL_HANDLE;
		VkDevice _gpuDevice;

		//wsi surface integration
		VkSurfaceKHR _surface;
		VkQueue _graphicsQueue;
		VkQueue _presentationQueue;
		
		//command buffers
		VkCommandPool _commandPool;
		std::vector<VkCommandBuffer> _commandBuffers;

		//Semaphores (denoted with s_)
		std::vector<VkSemaphore> _s_imageAvailable, _s_renderFinished;

		//Fences (denoted with a f_)
		std::vector<VkFence> _f_inFlight, _f_imagesInFlight;

		
		std::vector<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; //these are the absolute bare minimum required extensions to be considered suitable



		bool _isDeviceSuitable(VkPhysicalDevice device);//TODO: move to seperate factory class or something
		void _createLogicalDevice();
		void _createSwapChain();
		void _createImageViews();
		void _createRenderPass();
		void _createGraphicsPipeline();
		void _createFramebuffers();
		void _createCommandPool();
		void _createCommandBuffers();
		void _pickPhysicalDevice(VkInstance instance);
		VkShaderModule _createShaderModule(const std::vector<uint32_t>& code);
		SwapChainSupportDetails _querySwapChainSupport(VkPhysicalDevice device);
		QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);
		VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	public:

		inline const VkDevice getLogicalDevice() { return _gpuDevice; }
		inline const VkPhysicalDevice getPhysicalDevice() { return _gpu; }


		void init(VkInstance vkInstance, GLFWwindow* _windowRef);
		void dispose(VkInstance vkInstance);
	};

}