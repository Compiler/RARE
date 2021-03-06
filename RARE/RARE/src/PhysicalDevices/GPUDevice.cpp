#include "GPUDevice.h"

namespace Rare {



	void GPUDevice::init(VkInstance vkInstance, GLFWwindow* _windowRef) {
		if (glfwCreateWindowSurface(vkInstance, _windowRef, nullptr, &_surface) != VK_SUCCESS) {
			RARE_FATAL("Couldn't create window surface");
		}


		//begin picking physical device
		RARE_LOG("Pick Physical Device:\t\t Begin init");
		_pickPhysicalDevice(vkInstance);
		RARE_LOG("Pick Physical Device:\t\t Initialization complete\n");


		//begin creating logical device
		RARE_LOG("Create Logical Device:\t\t Begin init");
		_createLogicalDevice();
		RARE_LOG("Create Logical Device:\t\t Initialization complete\n");

		//begin creating swap chain
		RARE_LOG("Create Swap Chain:\t\t Begin init");
		_createSwapChain();
		RARE_LOG("Create Swap Chain:\t\t Initialization complete\n");


		//begin creating image views
		RARE_LOG("Create Image Views:\t\t Begin init");
		_createImageViews();
		RARE_LOG("Create Image Views:\t\t Initialization complete\n");

		//begin creating render pass
		RARE_LOG("Create Render Pass:\t\t Begin init");
		_createRenderPass();
		RARE_LOG("Create Render Pass:\t\t Initialization complete\n");

		//begin creating graphics pipeline
		RARE_LOG("Create Graphics Pipeline:\t Begin init");
		_createGraphicsPipeline();
		RARE_LOG("Create Graphics Pipeline:\t Initialization complete\n");

		//begin creating framebuffers
		RARE_LOG("Create Framebuffers:\t\t Begin init");
		_createFramebuffers();
		RARE_LOG("Create Framebuffers:\t\t Initialization complete\n");

		//begin creating command pool
		RARE_LOG("Create Command Pool:\t\t Begin init");
		//_createCommandPool();
		RARE_LOG("Create Command Pool:\t\t Initialization complete\n");

		//begin creating command buffers
		RARE_LOG("Create Command Buffers:\t Begin init");
		//_createCommandBuffers();
		RARE_LOG("Create Command Buffers:\t Initialization complete\n");

		//begin creating semaphores
		RARE_LOG("Create Synchronization Objects: Begin init");
		//_createSynchronizationObjects();
		RARE_LOG("Create Synchronization Objects: Initialization complete\n");

	}

	void GPUDevice::_pickPhysicalDevice(VkInstance instance) {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) RARE_FATAL("No PhysicalDevicesFound");
		RARE_DEBUG("{} physical device(s) found", deviceCount);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		std::multimap<int, VkPhysicalDevice> candidates;

		auto rateDeviceSuitability = [](VkPhysicalDevice device)->int {
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
			auto deviceName = deviceProperties.deviceName;
			int score = 0;

			// Discrete GPUs have a significant performance advantage
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 10;

			float val = (deviceProperties.limits.maxImageDimension2D);
			val = (1.0f - 0.0f) / (1000000.0f - -1000000.0f) * (val - -1000000.0f) + 0.0f;
			score += (int)(val * 9.0f);

			if (!deviceFeatures.geometryShader) {
				RARE_ERROR("Device '{}' doesn't support geometry shaders", deviceName);//bad but not fatal
				score -= 5;
			}
			if (!deviceFeatures.tessellationShader) {
				RARE_ERROR("Device '{}' doesn't support tesselation shaders", deviceName);
				score -= 2;
			}
			if (!deviceFeatures.multiViewport) {
				RARE_WARN("Device '{}' doesn't support multiple viewports ", deviceName);
				score -= 1;
			}
			return score;
		};
		for (const auto& device : devices) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			auto deviceName = deviceProperties.deviceName;
			//must be suitable to be ranked among competing devices
			if (_isDeviceSuitable(device)) {
				RARE_TRACE("Device '{}' was suitable as a graphics family candidate", deviceName);
				int score = rateDeviceSuitability(device);
				candidates.insert(std::make_pair(score, device));
			} else {
				RARE_WARN("Device '{}' was not suitable for graphics queue family", deviceName);
			}
		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0) {
			_gpu = candidates.rbegin()->second;
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(_gpu, &deviceProperties);
			RARE_DEBUG("PhysicalDevice assigned to '{}'", deviceProperties.deviceName);
		} else {
			RARE_FATAL("failed to find a suitable GPU!");

		}

		if (_gpu == VK_NULL_HANDLE) RARE_FATAL("Couldn't find a physical device");



	}

	void GPUDevice::_createLogicalDevice() {
		QueueFamilyIndices indices = _findQueueFamilies(_gpu);//find queue families for graphics and presentation
		if (!indices.isComplete()) RARE_WARN("Returned queues do not sufficiently cover the command requirements");

		VkDeviceQueueCreateInfo queueCreateInfo{};
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentationFamily.value() };
		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);

		}

		VkPhysicalDeviceFeatures deviceFeatures{};//specify which functionality we need -- come back later

		VkDeviceCreateInfo createInfo{ };
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = (uint_fast32_t)queueCreateInfos.size();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = (uint32_t)requiredDeviceExtensions.size();
		createInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();
		//if (_enableValidationLayers) {
		//	createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
		//	createInfo.ppEnabledLayerNames = _validationLayers.data();
		//} else
		//	createInfo.enabledLayerCount = 0;

		if (vkCreateDevice(_gpu, &createInfo, nullptr, &_gpuDevice) != VK_SUCCESS) RARE_FATAL("Couldn't create logical device");

		vkGetDeviceQueue(_gpuDevice, indices.presentationFamily.value(), 0, &_presentationQueue);//index 0 cuz one queue family (graphics)
		vkGetDeviceQueue(_gpuDevice, indices.graphicsFamily.value(), 0, &_graphicsQueue);

	}

	QueueFamilyIndices GPUDevice::_findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		RARE_DEBUG("Queue family count: {}", queueFamilyCount);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		int index = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsFamily = index;
			VkBool32 presentSupport = false;//remember
			vkGetPhysicalDeviceSurfaceSupportKHR(device, index, _surface, &presentSupport);
			if (presentSupport) indices.presentationFamily = index; //same queue family 
			if (indices.isComplete())
				break;
			index++;
		}

		return indices;
	}

	bool GPUDevice::_isDeviceSuitable(VkPhysicalDevice device) {

		QueueFamilyIndices indices = _findQueueFamilies(device);

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
		std::set<std::string> requiredExtensions(requiredDeviceExtensions.begin(), requiredDeviceExtensions.end());

		for (const auto& extension : availableExtensions) requiredExtensions.erase(extension.extensionName);

		bool swapChainAdequate = false;
		if (requiredExtensions.empty()) {

			SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

		}

		return indices.isComplete() && requiredExtensions.empty() && swapChainAdequate;
	}

	SwapChainSupportDetails GPUDevice::_querySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;
		/*Start of querying surface capabalities / properties*/
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);
		auto curDims = details.capabilities.currentExtent;
		RARE_DEBUG("\tSurface current dimensions supported: ({}, {})", curDims.width, curDims.height);
		RARE_DEBUG("\tSurface swapchain will support at least {} images in chain and at most {}", details.capabilities.minImageCount, details.capabilities.maxImageCount);
		/*End of querying surface capabalities / properties*/

		/*Start of querying supported surface formats*/
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
		}
		/*End of querying supported surface formats*/


		/*Start of querying supported presentation modes*/
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
		}
		/*End of querying supported presentation modes*/

		return details;
	}

	void GPUDevice::_createSwapChain() {
		SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(_gpu);

		VkSurfaceFormatKHR surfaceFormat = _chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = _chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = _chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		//create swapchain info
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = _surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;//resolution of the swap chain images
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;//the use of the images in this swapchain is to be directly rendered to

		QueueFamilyIndices indices = _findQueueFamilies(_gpu);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentationFamily.value() };

		if (indices.graphicsFamily != indices.presentationFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;//performant option. images are owned by one queue family at a time
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;//does the image drawn to the window allow other windows to show through below it
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;//enables clipping of pixels that are behind another window
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		//create swapchain
		if (vkCreateSwapchainKHR(_gpuDevice, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
			RARE_FATAL("Failed to create Swap Chain!");
		}

		//store swap chain image handles in _swapChainImages
		vkGetSwapchainImagesKHR(_gpuDevice, _swapChain, &imageCount, nullptr);
		_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(_gpuDevice, _swapChain, &imageCount, _swapChainImages.data());

		//store information about swap chain images for later use
		_swapChainImageFormat = surfaceFormat.format;
		_swapChainExtent = extent;

	}
	//choose from queuried(and provided) available formats
	VkSurfaceFormatKHR GPUDevice::_chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return availableFormats[0];
	}
	VkPresentModeKHR GPUDevice::_chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				RARE_DEBUG("Using mailbox mode as presentation mode");
				return availablePresentMode;
			}
		}
		RARE_DEBUG("Using FIFO mode as presentation mode");
		return VK_PRESENT_MODE_FIFO_KHR;

	}

	VkExtent2D GPUDevice::_chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		VkExtent2D returnedExtent;
		if (capabilities.currentExtent.width != UINT32_MAX) {
			returnedExtent = capabilities.currentExtent;

		} else {
			int WIDTH = 640;
			int HEIGHT = 480;
			VkExtent2D actualExtent = { WIDTH, HEIGHT };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
			returnedExtent = actualExtent;

		}
		RARE_DEBUG("Swap extent chosen: ({}, {})", returnedExtent.width, returnedExtent.height);
		return returnedExtent;
	}
	void GPUDevice::_createImageViews() {
		_swapChainImageViews.resize(_swapChainImages.size());
		for (int i = 0; i < _swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = _swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = _swapChainImageFormat;

			//color channels
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			//images purpose and which parts of image to access
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(_gpuDevice, &createInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS) {
				RARE_FATAL("Couldn't create image view object from image");
			}
		}
	}

	void GPUDevice::_createCommandPool() {
		QueueFamilyIndices qFamIndices = _findQueueFamilies(_gpu);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = qFamIndices.graphicsFamily.value();
		poolInfo.flags = 0;
		if (vkCreateCommandPool(_gpuDevice, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
			RARE_FATAL("Couldn't create Command Pool");
		}
	}

	void GPUDevice::_createCommandBuffers() {
		_commandBuffers.resize(_swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = _commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

		if (vkAllocateCommandBuffers(_gpuDevice, &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
			RARE_FATAL("Could not allocate command buffers");
		}
		for (size_t i = 0; i < _commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				RARE_FATAL("Could not begin recording command buffer");
			}
			VkRenderPassBeginInfo rpInfo{};
			rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			rpInfo.renderPass = _renderPass;
			rpInfo.framebuffer = _swapChainFramebuffers[i];
			rpInfo.renderArea.offset = { 0, 0 };
			rpInfo.renderArea.extent = _swapChainExtent;

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };//clear color used for VK_ATTACHMENT_LOAD_OP_CLEAR
			rpInfo.clearValueCount = 1;
			rpInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(_commandBuffers[i], &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
			vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);
			vkCmdEndRenderPass(_commandBuffers[i]);

			if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
				RARE_FATAL("Could not record command buffer");
			}
		}
	}

	/*
	-Info about framebuffer attachments that will be used while rendering
	-Number of color and depth buffers
	-How many samples will be used for each buffer
	-How buffer contents should be handled throughout the rendering operations
	*/
	void GPUDevice::_createRenderPass() {
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = _swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;//layout used for images that are presented to the swapchain

		//Create Subpasses
		VkAttachmentReference colAttRef{};
		colAttRef.attachment = 0;//index of attachment to reference
		colAttRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//layout used for images that are used as color attachments

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;//subpass bind for graphics rather than compute
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colAttRef;//referenced from fragment shader in the line "layout(location = 0) out vec4 outColor;"

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		//Create Render Pass
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(_gpuDevice, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
			RARE_FATAL("Failed to create render pass");
		}

	}

	void GPUDevice::_createFramebuffers() {
		_swapChainFramebuffers.resize(_swapChainImageViews.size());
		for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
			VkImageView attachments[] = { _swapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = _renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = _swapChainExtent.width;
			framebufferInfo.height = _swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(_gpuDevice, &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS)
				RARE_FATAL("Couldn't create framebuffer");

		}
	}
	VkShaderModule GPUDevice::_createShaderModule(const std::vector<uint32_t>& code) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = code.data();

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(_gpuDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			RARE_FATAL("Couldn't create shader module");
		}
		return shaderModule;

	}
	void GPUDevice::_createGraphicsPipeline() {

		//Shader Stage Creation
		/*
		-Shader modules that define the functionality of the programmable stages of the graphics pipeline
		*/
		auto vertexShaderCode = ShaderCompilation::CompileShaderSource(RARE_INTERNAL("shaders/VertexShader.vert"), ShaderCompilation::RARE_SHADER_TYPE::VERTEX);
		auto fragmentShaderCode = ShaderCompilation::CompileShaderSource(RARE_INTERNAL_SHADER("FragmentShader.frag"), ShaderCompilation::RARE_SHADER_TYPE::FRAGMENT);
		//auto vertexShaderCode1 = ShaderCompilation::ReadShaderSPV("src/shaders/VertexShader.spv");
		//auto fragmentShaderCode = ShaderCompilation::ReadShaderSPV("src/shaders/FragmentShader.spv");
		VkShaderModule vShaderMod = _createShaderModule(vertexShaderCode);
		VkShaderModule fShaderMod = _createShaderModule(fragmentShaderCode);//note: these arent needed after pipeline creation, so they are not class members

		VkPipelineShaderStageCreateInfo vShaderStageInfo{};
		vShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vShaderStageInfo.module = vShaderMod;
		vShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fShaderStageInfo{};
		fShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fShaderStageInfo.module = fShaderMod;
		fShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vShaderStageInfo, fShaderStageInfo };


		//Vertex Input Stage Creation
		VkPipelineVertexInputStateCreateInfo vInputInfo{};
		vInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vInputInfo.vertexBindingDescriptionCount = 0;
		vInputInfo.pVertexBindingDescriptions = nullptr;//vertex binding descriptions specify space between data and if data is per vert or per instance
		vInputInfo.vertexAttributeDescriptionCount = 0;
		vInputInfo.pVertexAttributeDescriptions = nullptr;//vertex attribute descriptions specify the attribute types and binding location of attributes that get passed to the vert shader

		VkPipelineInputAssemblyStateCreateInfo inputAssenblyInfo{};
		inputAssenblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssenblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssenblyInfo.primitiveRestartEnable = VK_FALSE;


		//Viewport State Creation
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)_swapChainExtent.width;
		viewport.height = (float)_swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = _swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;


		//Rasterization Stage Creation
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;//Vertex winding order
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;


		//Multisanpling State Creation
		VkPipelineMultisampleStateCreateInfo multisampler{};
		multisampler.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampler.sampleShadingEnable = VK_FALSE;
		multisampler.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampler.minSampleShading = 1.0f;
		multisampler.pSampleMask = nullptr;
		multisampler.alphaToCoverageEnable = VK_FALSE;
		multisampler.alphaToOneEnable = VK_FALSE;


		//Color Blending Options
		VkPipelineColorBlendAttachmentState colBlendAttach{};
		colBlendAttach.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colBlendAttach.blendEnable = VK_TRUE;
		colBlendAttach.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colBlendAttach.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colBlendAttach.colorBlendOp = VK_BLEND_OP_ADD;
		colBlendAttach.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colBlendAttach.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colBlendAttach.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colBlend{};
		colBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colBlend.logicOpEnable = VK_FALSE;//use bitwise operation for color blending. this will override blendEnable
		colBlend.logicOp = VK_LOGIC_OP_COPY;//specify bitwise operation for color blending
		colBlend.attachmentCount = 1;
		colBlend.pAttachments = &colBlendAttach;
		colBlend.blendConstants[0] = 0.0f;
		colBlend.blendConstants[1] = 0.0f;
		colBlend.blendConstants[2] = 0.0f;
		colBlend.blendConstants[3] = 0.0f;


		//Pipeline states that can be changed without recreating the pipeline
		VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_LINE_WIDTH
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;


		//Pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(_gpuDevice, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
			RARE_FATAL("Failed to create pipeline layout");
		}


		//Actual Pipeline Creation
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssenblyInfo;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampler;
		pipelineInfo.pColorBlendState = &colBlend;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pDynamicState = nullptr; //to be implemented later?
		pipelineInfo.layout = _pipelineLayout;
		pipelineInfo.renderPass = _renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(_gpuDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS) {
			RARE_FATAL("Failed to create graphics pipeline");
		}

		vkDestroyShaderModule(_gpuDevice, fShaderMod, nullptr);
		vkDestroyShaderModule(_gpuDevice, vShaderMod, nullptr);
	}


	void GPUDevice::dispose(VkInstance vkInstance) {
		vkDestroySurfaceKHR(vkInstance, _surface, nullptr);

		for (auto framebuffer : _swapChainFramebuffers) {
			vkDestroyFramebuffer(_gpuDevice, framebuffer, nullptr);
		}
		vkDestroyRenderPass(_gpuDevice, _renderPass, nullptr);
		for (auto imageView : _swapChainImageViews) {
			vkDestroyImageView(_gpuDevice, imageView, nullptr);
		}


		vkDestroySwapchainKHR(_gpuDevice, _swapChain, nullptr);
	}
}