#include "RareCore.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Rare {
	struct RareCore::QueueFamilyIndices {
		Optional<uint32_t> graphicsFamily;
		Optional<uint32_t> presentationFamily;

		//method to show whether or not a set of needed commands have be acquired 
		bool isComplete() {
			return graphicsFamily.has_value && presentationFamily.has_value;
		}
	};


	RareCore::RareCore() :_windowRefName("Default Name"), _validationLayers({ "VK_LAYER_KHRONOS_validation" }) { _coreShouldClose = false; }
	RareCore::RareCore(const char* windowName) : _windowRefName(windowName), _validationLayers({ "VK_LAYER_KHRONOS_validation" }) { _coreShouldClose = false; }
	RareCore::RareCore(const char* windowName, const std::vector<const char*> validationLayers)
		: _windowRefName(windowName), _validationLayers(validationLayers) {
		_coreShouldClose = false;
	}

	void RareCore::init() {
		{
			std::unordered_map<uint32_t, std::string> map = { {0b00001, "bit 1"},{0b00010, "bit 2"},{0b00011, "bit 3"},{0b00100, "bit 4"},{0b00101, "bit 5"},{0b00110, "bit 6"},{0b00111, "bit 7"},{0b01000, "bit 8"} };
			uint32_t flags = 0b00001 | 0b00010 | 0b01011;
			uint32_t runningFlag = flags;
			while (flags != 0) {
				RARE_LOG("Flag: {}", flags);
				if (flags % 2 == 0) {
					runningFlag <= 1;
				} else runningFlag ^= 1;
				flags -= 1;
			}
		}
		//begin logger initialization
		Rare::Logger::init();

		RARE_LOG("Logger:\t\t\t Initialization complete");

		//begin glfw initialization
		RARE_LOG("GLFW:\t\t\t\t Begin init");
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		_windowRef = glfwCreateWindow(_WIDTH, _HEIGHT, _windowRefName, NULL, NULL);

		glfwSetFramebufferSizeCallback(_windowRef, GLFWCallbacks::framebufferResizeCallback);
		glfwSetKeyCallback(_windowRef, GLFWCallbacks::keyCallback);
		glfwSetCursorPosCallback(_windowRef, GLFWCallbacks::cursorPositionCallback);
		glfwSetMouseButtonCallback(_windowRef, GLFWCallbacks::mouseClickCallback);

		glfwSetWindowUserPointer(_windowRef, this);

		glfwSetWindowPos(_windowRef, _WIDTH * 2, _HEIGHT);
		glfwMakeContextCurrent(_windowRef);
		RARE_LOG("GLFW:\t\t\t\t Initialization complete\n");

		RARE_LOG("Vulkan:\t\t\t Begin init");
		_createVkInstance();
		RARE_LOG("Vulkan:\t\t\t Initialization complete\n");

		RARE_LOG("Validation Layers:\t\t Begin init");
		_setupDebugMessenger();
		RARE_LOG("Validation Layers:\t\t Initialization complete\n");

		RARE_LOG("Create Surface:\t\t Begin init");
		_createSurface();
		RARE_LOG("Create Surface:\t\t Initialization complete\n");

		RARE_LOG("Pick Physical Device:\t\t Begin init");
		_pickPhysicalDevice();
		RARE_LOG("Pick Physical Device:\t\t Initialization complete\n");

		RARE_LOG("Create Logical Device:\t\t Begin init");
		_createLogicalDevice();
		RARE_LOG("Create Logical Device:\t\t Initialization complete\n");

		RARE_LOG("Create Swap Chain:\t\t Begin init");
		_createSwapChain();
		RARE_LOG("Create Swap Chain:\t\t Initialization complete\n");

		RARE_LOG("Create Image Views:\t\t Begin init");
		_createImageViews();
		RARE_LOG("Create Image Views:\t\t Initialization complete\n");

		RARE_LOG("Create Render Pass:\t\t Begin init");
		_createRenderPass();
		RARE_LOG("Create Render Pass:\t\t Initialization complete\n");

		RARE_LOG("Create Descriptor Set Layout:\t Begin init");
		_createDescriptorSetLayout();
		RARE_LOG("Create Descriptor Set Layout:\t Initialization complete\n");

		RARE_LOG("Create Graphics Pipeline:\t Begin init");
		_createGraphicsPipeline();
		RARE_LOG("Create Graphics Pipeline:\t Initialization complete\n");

		RARE_LOG("Create Command Pool:\t\t Begin init");
		_createCommandPool();
		RARE_LOG("Create Command Pool:\t\t Initialization complete\n");
		
		RARE_LOG("Create Color Resources:\t\t Begin init");
		_createColorResources();
		RARE_LOG("Create Color Resources:\t\t Initialization complete\n");

		RARE_LOG("Create Depth Resources:\t\t Begin init");
		_createDepthResources();
		RARE_LOG("Create Depth Resources:\t\t Initialization complete\n");

		RARE_LOG("Create Framebuffers:\t\t Begin init");
		_createFramebuffers();
		RARE_LOG("Create Framebuffers:\t\t Initialization complete\n");

		RARE_LOG("Create Texture Image:\t\t Begin init");
		_createTextureImage();
		RARE_LOG("Create Texture Image:\t\t Initialization complete\n");

		RARE_LOG("Create Texture Image View:\t\t Begin init");
		_createTextureImageView();
		RARE_LOG("Create Texture Image View:\t\t Initialization complete\n");

		RARE_LOG("Create Texture Sampler:\t\t Begin init");
		_createTextureSampler();
		RARE_LOG("Create Texture Sampler:\t\t Initialization complete\n");

		RARE_LOG("Load Model:\t\t Begin init");
		_loadModel();
		RARE_LOG("Load Model:\t\t Load complete\n");

		RARE_LOG("Create Vertex Buffer:\t\t Begin init");
		BufferFactory::createVertexBuffer(_vkContext, _vertices, _vertexBuffer, _vertexBufferMemory);
		RARE_LOG("Create Vertex Buffer:\t\t Initialization complete\n");

		RARE_LOG("Create Index Buffer:\t\t Begin init");
		BufferFactory::createIndexBuffer(_vkContext, _indices, _indexBuffer, _indexBufferMemory);
		RARE_LOG("Create Index Buffer:\t\t Initialization complete\n");

		RARE_LOG("Create Uniform Buffer:\t\t Begin init");
		_createUniformBuffers();
		RARE_LOG("Create Uniform Buffer:\t\t Initialization complete\n");

		RARE_LOG("Create Descriptor Pool:\t\t Begin init");
		_createDescriptorPool();
		RARE_LOG("Create Descriptor Pool:\t\t Initialization complete\n");

		RARE_LOG("Create Descriptor Sets:\t\t Begin init");
		_createDescriptorSets();
		RARE_LOG("Create Descriptor Sets:\t\t Initialization complete\n");

		RARE_LOG("Create Command Buffers:\t Begin init");
		_createCommandBuffers();
		RARE_LOG("Create Command Buffers:\t Initialization complete\n");

		RARE_LOG("Create Synchronization Objects: Begin init");
		_createSynchronizationObjects();
		RARE_LOG("Create Synchronization Objects: Initialization complete\n");


		RARE_LOG("Initialization complete");
	}


	void RareCore::_createColorResources() {
		VkFormat colorFormat = _swapChainImageFormat;

		_createImage(_swapChainExtent.width, _swapChainExtent.height, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _colorImage, _colorImageMemory, 1, _msaaSamples);
		_colorImageView = _createImageView(_colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	void RareCore::_generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(_vkContext.physicalDevice, imageFormat, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
			RARE_FATAL("texture image format does not support linear blitting!");
		}

		VkCommandBuffer commandBuffer = CmdBufferFactory::beginOneoffCommands(_vkContext);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = texWidth;
		int32_t mipHeight = texHeight;

		for (uint32_t i = 1; i < mipLevels; i++) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		CmdBufferFactory::endOneoffCommands(_vkContext, commandBuffer);
	}
	void RareCore::_createDepthResources() {
		VkFormat depthFormat = _findDepthFormat();
		_createImage(_swapChainExtent.width, _swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
										_depthImage, _depthImageMemory, 1, _msaaSamples);
		_depthImageView = _createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
		//TODO: Consider Explicitly transitioning the depth image to a depth attachment
	}

	VkFormat  RareCore::_findSupportedBitFormats(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(_vkContext.physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			} else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}
		RARE_FATAL("failed to find supported format!");
	}

	VkImageView RareCore::_createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.subresourceRange.aspectMask = aspectFlags;

		VkImageView imageView;
		if (vkCreateImageView(_vkContext.logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			RARE_FATAL("failed to create texture image view!");
		}

		return imageView;
	}
	void RareCore::_createTextureImageView() {
		_textureImageView = _createImageView(_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, _mipLevels);
	}

	void RareCore::_loadModel() {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string wrn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &wrn, &err, _MODEL_PATH.c_str())) {
			RARE_FATAL("Tinyobj Warn: {} \n Tinyobj Error: {}", wrn, err);
		}

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				VertexData vertex{};
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				_vertices.push_back(vertex);
				_indices.push_back(_indices.size());
			}
		}
	}

	void RareCore::_createTextureSampler() {
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0; //set to 0'static_cast<float>(_mipLevels / 2)' for normal rendering
		samplerInfo.maxLod = static_cast<float>(_mipLevels);
		samplerInfo.mipLodBias = 0.0f;

		if (vkCreateSampler(_vkContext.logicalDevice, &samplerInfo, nullptr, &_textureSampler) != VK_SUCCESS) {
			RARE_FATAL("failed to create texture sampler!");
		}
	}

	void RareCore::_copyBufferToImage(VkBuffer buff, VkImage img, uint32_t width, uint32_t height) {
		VkCommandBuffer cbuff = CmdBufferFactory::beginOneoffCommands(_vkContext);
		RARE_DEBUG("Began one time command");

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		RARE_DEBUG("Created Buffer image copy region");

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { width, height, 1 };

		vkCmdCopyBufferToImage(cbuff, buff, img, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		RARE_DEBUG("send cmd to coppy buffer to image");

		CmdBufferFactory::endOneoffCommands(_vkContext, cbuff);
		RARE_DEBUG("Ended one time command");

	}

	void RareCore::_transitionImageLayout(VkImage img, VkFormat fmt, VkImageLayout olay, VkImageLayout nlay, uint32_t mipLevels) {
		VkCommandBuffer cbt = CmdBufferFactory::beginOneoffCommands(_vkContext);//cbt stands for "command buffer : transition" 
		RARE_DEBUG("Began one time command");

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = olay;
		barrier.newLayout = nlay;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = img;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = 0;
		RARE_DEBUG("Created VkImageMemoryBarrier");

		VkPipelineStageFlags srcStage, dstStage;

		if (olay == VK_IMAGE_LAYOUT_UNDEFINED && nlay == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if (olay == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && nlay == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else {
			RARE_FATAL("unsupported layout transition");
		}
		RARE_DEBUG("Set properties");

		vkCmdPipelineBarrier(cbt,
			srcStage, dstStage,   //pipeline stage flags  (wait for this stage to finish), (these stages wait for you to finish)
			0,						//dependency flags
			0, nullptr,				//memory barrier
			0, nullptr,				//buffer memory barrier
			1, &barrier);			//image memory barrier
		RARE_DEBUG("send cmd PipelineBarrier");

		CmdBufferFactory::endOneoffCommands(_vkContext, cbt);
		RARE_DEBUG("Ended one time command");


	}

	void RareCore::_createTextureImage() {
		int texWidth, texHeight, texChannels;
		unsigned char* pixels = FileLoaderFactory::loadImage(_TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels);
		VkDeviceSize imageSize = texWidth * texHeight * 4;
		_mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
		if (!pixels) {
			RARE_FATAL("Failed to load texture image");
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		BufferFactory::createBuffer(_vkContext, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		//map pixels from loadImage to buffer
		void* data;
		vkMapMemory(_vkContext.logicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(_vkContext.logicalDevice, stagingBufferMemory);
		FileLoaderFactory::free(pixels);

		_createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT  | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _textureImage, _textureImageMemory, _mipLevels);
		RARE_DEBUG("Created image");
		_transitionImageLayout(_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, _mipLevels);
		RARE_DEBUG("Transitioned image layout image");
		_copyBufferToImage(stagingBuffer, _textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		RARE_DEBUG("Coppied buffer to  image");
		//_transitionImageLayout(_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, _mipLevels);
		//RARE_DEBUG("Transitioned image layout image");


		vkDestroyBuffer(_vkContext.logicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(_vkContext.logicalDevice, stagingBufferMemory, nullptr);
		_generateMipmaps(_textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, _mipLevels);

	}

	void RareCore::_createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, uint32_t mipLevels, VkSampleCountFlagBits numSamples) {
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(width);
		imageInfo.extent.height = static_cast<uint32_t>(height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;//potentially not supported by all graphics hardware. keep an eye on this for multiplat building
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = numSamples;
		imageInfo.flags = 0;

		if (vkCreateImage(_vkContext.logicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			RARE_FATAL("Failed to create image");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(_vkContext.logicalDevice, image, &memRequirements);

		VkMemoryAllocateInfo aInfo{};
		aInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		aInfo.allocationSize = memRequirements.size;
		aInfo.memoryTypeIndex = BufferFactory::findMemoryType(_vkContext, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(_vkContext.logicalDevice, &aInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			RARE_FATAL("Failed to allocate image memory");
		}

		vkBindImageMemory(_vkContext.logicalDevice, image, imageMemory, 0);
	}

	void RareCore::_createDescriptorSets() {
		std::vector<VkDescriptorSetLayout> layouts(_swapChainImages.size(), _descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(_swapChainImages.size());
		allocInfo.pSetLayouts = layouts.data();

		_descriptorSets.resize(_swapChainImages.size());
		if (vkAllocateDescriptorSets(_vkContext.logicalDevice, &allocInfo, _descriptorSets.data()) != VK_SUCCESS) {
			RARE_FATAL("Failed to allocate descriptor sets");
		}


		for (size_t i = 0; i < _swapChainImages.size(); i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = _uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = _textureImageView;
			imageInfo.sampler = _textureSampler;


			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = _descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = _descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(_vkContext.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}


	}

	void RareCore::_createDescriptorPool() {
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t>(_swapChainImages.size());
		

		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(_swapChainImages.size());
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(_swapChainImages.size());

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(_swapChainImages.size());

		if (vkCreateDescriptorPool(_vkContext.logicalDevice, &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
			RARE_FATAL("Failed to create descriptor pool");
		}
	}

	/*A better way than sending ubos is to use push constants in a small buffer*/
	void RareCore::_updateUniformBuffer(uint32_t imageIndex) {
		float time = glfwGetTime();
		UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), /*time **/ glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), _swapChainExtent.width / (float)_swapChainExtent.height, 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;
		ubo.time = time;
		void* data;
		vkMapMemory(_vkContext.logicalDevice, _uniformBuffersMemory[imageIndex], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(_vkContext.logicalDevice, _uniformBuffersMemory[imageIndex]);


	}

	void RareCore::_createUniformBuffers() {
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		//ubo per swap chain image
		_uniformBuffers.resize(_swapChainImages.size());
		_uniformBuffersMemory.resize(_swapChainImages.size());

		for (size_t i = 0; i < _swapChainImages.size(); i++) {
			BufferFactory::createBuffer(_vkContext, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _uniformBuffers[i], _uniformBuffersMemory[i]);
		}
	}

	void RareCore::_createDescriptorSetLayout() {
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(_vkContext.logicalDevice, &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS) {
			RARE_FATAL("failed to create descriptor set layout");
		}

	}

	void RareCore::_createVkInstance() {

		//check for validaton layer compatability issues
		if (_enableValidationLayers && !_checkValidationLayerSupport()) {
			RARE_FATAL("Validation Layers Unsupported");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Core";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "RARE";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;


		auto glfwExtensions = _getRequiredExtensions();

		uint32_t vkExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(vkExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, extensions.data());


		VkInstanceCreateInfo createInfo{};//tells vulkan driver which global extensions and validation layers to use
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
		createInfo.ppEnabledExtensionNames = glfwExtensions.data();
		createInfo.enabledLayerCount = 0;
		if (_enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
			createInfo.ppEnabledLayerNames = _validationLayers.data();
		} else {
			createInfo.enabledLayerCount = 0;
		}

		//Validation Layer stuff
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (_enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
			createInfo.ppEnabledLayerNames = _validationLayers.data();

			_populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		} else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}


		//nullptr cuz no custom allocator rn
		if (vkCreateInstance(&createInfo, nullptr, &(_vkContext.vkInstance)) != VK_SUCCESS)
			RARE_FATAL("Failed to create vkInstance");




		RARE_DEBUG("{} extension(s) supported", vkExtensionCount);
		RARE_DEBUG("{} glfw extension(s) required", static_cast<uint32_t>(glfwExtensions.size()));

	}

	void RareCore::_createSurface() {
		if (glfwCreateWindowSurface(_vkContext.vkInstance, _windowRef, nullptr, &_surface) != VK_SUCCESS) {
			RARE_FATAL("Couldn't create window surface");
		}
	}

	void RareCore::_createLogicalDevice() {
		QueueFamilyIndices indices = _findQueueFamilies(_vkContext.physicalDevice);//find queue families for graphics and presentation
		if (!indices.isComplete()) RARE_WARN("Returned queues do not sufficiently cover the command requirements");

		VkDeviceQueueCreateInfo queueCreateInfo{};
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value, indices.presentationFamily.value };
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
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.sampleRateShading = VK_TRUE;
		VkDeviceCreateInfo createInfo{ };
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = (uint_fast32_t)queueCreateInfos.size();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = (uint32_t)requiredDeviceExtensions.size();
		createInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();
		if (_enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
			createInfo.ppEnabledLayerNames = _validationLayers.data();
		} else
			createInfo.enabledLayerCount = 0;

		if (vkCreateDevice(_vkContext.physicalDevice, &createInfo, nullptr, &(_vkContext.logicalDevice)) != VK_SUCCESS) RARE_FATAL("Couldn't create logical device");

		vkGetDeviceQueue(_vkContext.logicalDevice, indices.presentationFamily.value, 0, &_presentationQueue);
		vkGetDeviceQueue(_vkContext.logicalDevice, indices.graphicsFamily.value, 0, &(_vkContext.graphicsQueue));

	}

	void RareCore::_createSwapChain() {
		SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(_vkContext.physicalDevice);

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

		QueueFamilyIndices indices = _findQueueFamilies(_vkContext.physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value, indices.presentationFamily.value };

		if (indices.graphicsFamily.value != indices.presentationFamily.value) {
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
		if (vkCreateSwapchainKHR(_vkContext.logicalDevice, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
			RARE_FATAL("Failed to create Swap Chain!");
		}

		//store swap chain image handles in _swapChainImages
		vkGetSwapchainImagesKHR(_vkContext.logicalDevice, _swapChain, &imageCount, nullptr);
		_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(_vkContext.logicalDevice, _swapChain, &imageCount, _swapChainImages.data());

		//store information about swap chain images for later use
		_swapChainImageFormat = surfaceFormat.format;
		_swapChainExtent = extent;

	}

	void RareCore::update() {
		static double start = glfwGetTime();
		static double delta;
		glfwPollEvents();//assign this to a daemon thread and lock event manager to synch assignments
		//_coreShouldClose = (delta = glfwGetTime() - start) >= 44 ? true : false;
		_coreShouldClose = glfwWindowShouldClose(_windowRef);
		if (InputManager::isKeyPressed(KeyCodes::KEY_A)) RARE_LOG("A Pressed");


		InputManager::clear();
	}

	void RareCore::render() {
		static int frameCount = 0;
		static double startFPS = glfwGetTime();
		static double startMS = glfwGetTime();
		static double deltaFPS;
		static double deltaMS;
		deltaMS = glfwGetTime() - startMS;
		startMS = glfwGetTime();

		//uncomment for frame time comparisons
		//RARE_TRACE("MS: {}", deltaMS); 

		frameCount++;
		deltaFPS = glfwGetTime() - startFPS;
		if (deltaFPS >= 1) {
			if (FPS_COUNTER_LOGGED) {
				RARE_WARN("FPS: {}", frameCount);
				RARE_WARN("MS: {}", deltaMS);
			}
			frameCount = 0; startFPS = glfwGetTime();
			deltaFPS = 0;
		}
		vkWaitForFences(_vkContext.logicalDevice, 1, &_f_inFlight[_currentFrame], VK_TRUE, UINT64_MAX);


		uint32_t imageIndex;
		VkResult acquireResult = vkAcquireNextImageKHR(_vkContext.logicalDevice, _swapChain, UINT64_MAX, _s_imageAvailable[_currentFrame], VK_NULL_HANDLE, &imageIndex);
		/*This result means that the swap chain is no longer in a condition sufficient enough to continue using(something changed)*/
		if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR || acquireResult == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			_recreateSwapChain();//go back to section
			RARE_DEBUG("Swap chain recreated");
		} else if (acquireResult != VK_SUCCESS) {
			RARE_FATAL("Swap chain image couldn't be presented");
		}
		//check if previous frame is using this image
		if (_f_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(_vkContext.logicalDevice, 1, &_f_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		//mark image as being used by current frame
		_f_imagesInFlight[imageIndex] = _f_inFlight[_currentFrame];

		_updateUniformBuffer(imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore s_wait[] = { _s_imageAvailable[_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = s_wait;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_commandBuffers[imageIndex];
		VkSemaphore s_signal[] = { _s_renderFinished[_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = s_signal;

		vkResetFences(_vkContext.logicalDevice, 1, &_f_inFlight[_currentFrame]);

		if (vkQueueSubmit(_vkContext.graphicsQueue, 1, &submitInfo, _f_inFlight[_currentFrame]) != VK_SUCCESS)
			RARE_FATAL("Could not submit draw command buffer");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = s_signal;
		VkSwapchainKHR swapChains[] = { _swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(_presentationQueue, &presentInfo);

		vkQueueWaitIdle(_presentationQueue);

		_currentFrame = (_currentFrame + 1) % _MAX_FRAMES_IN_FLIGHT;
	}

	void RareCore::waitIdle() {
		vkDeviceWaitIdle(_vkContext.logicalDevice);
	}

	void RareCore::dispose() {
		_cleanupSwapChain();

		vkDestroyImageView(_vkContext.logicalDevice, _textureImageView, nullptr);

		vkDestroyImage(_vkContext.logicalDevice, _textureImage, nullptr);
		vkFreeMemory(_vkContext.logicalDevice, _textureImageMemory, nullptr);

		vkDestroyDescriptorSetLayout(_vkContext.logicalDevice, _descriptorSetLayout, nullptr);

		vkDestroyBuffer(_vkContext.logicalDevice, _indexBuffer, nullptr);
		vkFreeMemory(_vkContext.logicalDevice, _indexBufferMemory, nullptr);
		vkDestroyBuffer(_vkContext.logicalDevice, _vertexBuffer, nullptr);
		vkFreeMemory(_vkContext.logicalDevice, _vertexBufferMemory, nullptr);

		for (size_t i = 0; i < _MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(_vkContext.logicalDevice, _s_imageAvailable[i], nullptr);
			vkDestroySemaphore(_vkContext.logicalDevice, _s_renderFinished[i], nullptr);
			vkDestroyFence(_vkContext.logicalDevice, _f_inFlight[i], nullptr);

		}

		vkDestroyCommandPool(_vkContext.logicalDevice, _vkContext.commandPool, nullptr);

		vkDestroyDevice(_vkContext.logicalDevice, nullptr);

		if (_enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(_vkContext.vkInstance, _debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(_vkContext.vkInstance, _surface, nullptr);
		vkDestroyInstance(_vkContext.vkInstance, nullptr);

		glfwDestroyWindow(_windowRef);

		glfwTerminate();


		RARE_LOG("{} Window closed", _windowRefName);
	}

	bool RareCore::_isDeviceSuitable(VkPhysicalDevice device) {

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
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
		return indices.isComplete() && requiredExtensions.empty() && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	void RareCore::_pickPhysicalDevice() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_vkContext.vkInstance, &deviceCount, nullptr);
		if (deviceCount == 0) RARE_FATAL("No PhysicalDevicesFound");
		RARE_DEBUG("{} physical device(s) found", deviceCount);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_vkContext.vkInstance, &deviceCount, devices.data());

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
			_vkContext.physicalDevice = candidates.rbegin()->second;
			_msaaSamples = getMaxUsableSampleCount();
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(_vkContext.physicalDevice, &deviceProperties);
			RARE_DEBUG("PhysicalDevice assigned to '{}'", deviceProperties.deviceName);
		} else {
			RARE_FATAL("failed to find a suitable GPU!");
		}

		if (_vkContext.physicalDevice == VK_NULL_HANDLE) RARE_FATAL("Couldn't find a physical device");



	}

	RareCore::QueueFamilyIndices RareCore::_findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		RARE_DEBUG("Queue family count: {}", queueFamilyCount);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		int index = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily.value = index;
				indices.graphicsFamily.has_value = true;
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, index, _surface, &presentSupport);
			if (presentSupport) {
				indices.presentationFamily.value = index; //same queue family 
				indices.presentationFamily.has_value = true; //same queue family 
			}
			if (indices.isComplete())
				break;
			index++;
		}

		return indices;
	}

	std::vector<const char*> RareCore::_getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (_enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
	SwapChainSupportDetails RareCore::_querySwapChainSupport(VkPhysicalDevice device) {
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

	//choose from queuried(and provided) available formats
	VkSurfaceFormatKHR RareCore::_chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return availableFormats[0];
	}
	VkPresentModeKHR RareCore::_chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				RARE_DEBUG("Using mailbox mode as presentation mode");
				return availablePresentMode;
			}
		}
		RARE_DEBUG("Using FIFO mode as presentation mode");
		return VK_PRESENT_MODE_FIFO_KHR;

	}

	VkExtent2D RareCore::_chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		VkExtent2D returnedExtent;
		if (capabilities.currentExtent.width != UINT32_MAX) {
			returnedExtent = capabilities.currentExtent;

		} else {
			int currentWidth, currentHeight;
			glfwGetFramebufferSize(_windowRef, &currentWidth, &currentHeight);

			VkExtent2D actualExtent = { static_cast<uint32_t>(currentWidth),  static_cast<uint32_t>(currentHeight) };
			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
			returnedExtent = actualExtent;

		}
		RARE_DEBUG("Swap extent chosen: ({}, {})", returnedExtent.width, returnedExtent.height);
		return returnedExtent;
	}

	void RareCore::_createImageViews() {
		_swapChainImageViews.resize(_swapChainImages.size());
		for (int i = 0; i < _swapChainImages.size(); i++) {
			_swapChainImageViews[i] = _createImageView(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	/*
	-Info about framebuffer attachments that will be used while rendering
	-Number of color and depth buffers
	-How many samples will be used for each buffer
	-How buffer contents should be handled throughout the rendering operations
	*/
	void RareCore::_createRenderPass() {
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = _swapChainImageFormat;
		colorAttachment.samples = _msaaSamples;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//VK_ATTACHMENT_LOAD_OP_LOAD
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		//colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		//colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//layout used for images that are presented to the swapchain

		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = _findDepthFormat();
		depthAttachment.samples = _msaaSamples;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachmentResolve{};
		colorAttachmentResolve.format = _swapChainImageFormat;
		colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		//Create Subpasses
		VkAttachmentReference colAttRef{};
		colAttRef.attachment = 0;//index of attachment to reference
		colAttRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//layout used for images that are used as color attachments

		VkAttachmentReference depAttRef{};
		depAttRef.attachment = 1;//index of attachment to reference
		depAttRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;//layout used for images that are used as depth attachments

		VkAttachmentReference colorAttachmentResolveRef{};
		colorAttachmentResolveRef.attachment = 2;
		colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;//subpass bind for graphics rather than compute
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colAttRef;//referenced from fragment shader in the line "layout(location = 0) out vec4 outColor;"
		subpass.pDepthStencilAttachment = &depAttRef;
		subpass.pResolveAttachments = &colorAttachmentResolveRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		//Create Render Pass
		std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(_vkContext.logicalDevice, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
			RARE_FATAL("Failed to create render pass");
		}

	}

	void RareCore::_cleanupSwapChain() {
		RARE_LOG("Beginning cleaning of swap chain");
		vkDestroyImageView(_vkContext.logicalDevice, _colorImageView, nullptr);
		vkDestroyImage(_vkContext.logicalDevice, _colorImage, nullptr);
		vkFreeMemory(_vkContext.logicalDevice, _colorImageMemory, nullptr);
		for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {

			vkDestroyFramebuffer(_vkContext.logicalDevice, _swapChainFramebuffers[i], nullptr);
		}
		vkFreeCommandBuffers(_vkContext.logicalDevice, _vkContext.commandPool, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());


		vkDestroyPipeline(_vkContext.logicalDevice, _graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(_vkContext.logicalDevice, _pipelineLayout, nullptr);
		vkDestroyRenderPass(_vkContext.logicalDevice, _renderPass, nullptr);

		vkDestroyImageView(_vkContext.logicalDevice, _depthImageView, nullptr);
		vkDestroyImage(_vkContext.logicalDevice, _depthImage, nullptr);
		vkFreeMemory(_vkContext.logicalDevice, _depthImageMemory, nullptr);
		for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
			vkDestroyImageView(_vkContext.logicalDevice, _swapChainImageViews[i], nullptr);
		}
		vkDestroySwapchainKHR(_vkContext.logicalDevice, _swapChain, nullptr);

		for (size_t i = 0; i < _swapChainImages.size(); i++) {
			vkDestroyBuffer(_vkContext.logicalDevice, _uniformBuffers[i], nullptr);
			vkFreeMemory(_vkContext.logicalDevice, _uniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorPool(_vkContext.logicalDevice, _descriptorPool, nullptr);

		RARE_LOG("Ended cleaning of swap chain");

	}

	void RareCore::_recreateSwapChain() {

		/*This code checks for width/height to be 0 (i.e. the window is minimized) and then waits for it to not be minimized*/
		int currentFrameWidth = 0, currentFrameHeight = 0;
		glfwGetFramebufferSize(_windowRef, &currentFrameWidth, &currentFrameHeight);
		while (currentFrameWidth == 0 || currentFrameHeight == 0) {
			glfwGetFramebufferSize(_windowRef, &currentFrameWidth, &currentFrameHeight);
			glfwWaitEvents();
		}



		/*Being swap chain recreation*/
		vkDeviceWaitIdle(_vkContext.logicalDevice);

		_cleanupSwapChain();

		_createSwapChain();
		_createImageViews();
		_createRenderPass(); //depends on format of swap chain images
		_createGraphicsPipeline();
		_createColorResources();
		_createDepthResources();
		_createFramebuffers();
		_createUniformBuffers();
		_createDescriptorPool();
		_createDescriptorSets();
		_createCommandBuffers();


	}

	void RareCore::_createGraphicsPipeline() {

		//Shader Stage Creation
		/*
		-Shader modules that define the functionality of the programmable stages of the graphics pipeline
		*/
		auto vertexShaderCode = ShaderCompilation::CompileShaderSource(RARE_INTERNAL_SHADER("VertexShader.vert"), ShaderCompilation::RARE_SHADER_TYPE::VERTEX);
		auto fragmentShaderCode = ShaderCompilation::CompileShaderSource(RARE_INTERNAL_SHADER("FragmentShader.frag"/*"RayMarching.frag"*/), ShaderCompilation::RARE_SHADER_TYPE::FRAGMENT);
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


		//vertex shader attribute descriptions
		auto bindingDescription = VertexData::getBindingDescription();
		auto attributeDescriptions = VertexData::getAttributeDescriptions();

		//Vertex Input Stage Creation
		VkPipelineVertexInputStateCreateInfo vInputInfo{};
		vInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vInputInfo.vertexBindingDescriptionCount = 1;
		vInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vInputInfo.pVertexBindingDescriptions = &bindingDescription;//vertex binding descriptions specify space between data and if data is per vert or per instance
		vInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();//vertex attribute descriptions specify the attribute types and binding location of attributes that get passed to the vert shader

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
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;//VK_CULL_MODE_NONE
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE ;//Vertex winding order
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;


		//Multisanpling State Creation
		VkPipelineMultisampleStateCreateInfo multisampler{};
		multisampler.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampler.sampleShadingEnable = VK_TRUE;
		multisampler.rasterizationSamples = _msaaSamples;
		multisampler.minSampleShading = 0.2f;
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


		//depth stencil creation
		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;//lower depth value = closer
		depthStencil.depthBoundsTestEnable = VK_FALSE;//culls fragments that fall out of depth range?
		depthStencil.minDepthBounds = 0.0f;
		depthStencil.maxDepthBounds = 1.0f;
		depthStencil.stencilTestEnable = VK_FALSE;
		//depthStencil.front{};
		//depthStencil.back{};

		//Pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &_descriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(_vkContext.logicalDevice, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
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
		pipelineInfo.pDepthStencilState = &depthStencil;

		if (vkCreateGraphicsPipelines(_vkContext.logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS) {
			RARE_FATAL("Failed to create graphics pipeline");
		}

		vkDestroyShaderModule(_vkContext.logicalDevice, fShaderMod, nullptr);
		vkDestroyShaderModule(_vkContext.logicalDevice, vShaderMod, nullptr);
	}

	VkShaderModule RareCore::_createShaderModule(const std::vector<uint32_t>& code) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = code.data();

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(_vkContext.logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			RARE_FATAL("Couldn't create shader module");
		}
		return shaderModule;

	}

	void RareCore::_createFramebuffers() {
		_swapChainFramebuffers.resize(_swapChainImageViews.size());
		for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
			std::array<VkImageView, 3> attachments = { _colorImageView, _depthImageView, _swapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = _renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = _swapChainExtent.width;
			framebufferInfo.height = _swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(_vkContext.logicalDevice, &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS)
				RARE_FATAL("Couldn't create framebuffer");

		}
	}

	void RareCore::_createCommandPool() {
		QueueFamilyIndices qFamIndices = _findQueueFamilies(_vkContext.physicalDevice);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = qFamIndices.graphicsFamily.value;
		poolInfo.flags = 0;
		if (vkCreateCommandPool(_vkContext.logicalDevice, &poolInfo, nullptr, &(_vkContext.commandPool)) != VK_SUCCESS) {
			RARE_FATAL("Couldn't create Command Pool");
		}
		RARE_LOG("Command pool for graphics	queue family created");
	}

	void RareCore::_createCommandBuffers() {
		_commandBuffers.resize(_swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = _vkContext.commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

		if (vkAllocateCommandBuffers(_vkContext.logicalDevice, &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
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

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };//clear color used for VK_ATTACHMENT_LOAD_OP_CLEAR
			rpInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			rpInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(_commandBuffers[i], &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);

			VkBuffer vertexBuffers[] = { _vertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(_commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(_commandBuffers[i], _indexBuffer, 0, VK_INDEX_TYPE_UINT32);

			vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSets[i], 0, nullptr);

			vkCmdDrawIndexed(_commandBuffers[i], static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);
			vkCmdEndRenderPass(_commandBuffers[i]);

			if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
				RARE_FATAL("Could not record command buffer");
			}
		}
	}

	void RareCore::_createSynchronizationObjects() {
		_s_imageAvailable.resize(_MAX_FRAMES_IN_FLIGHT);
		_s_renderFinished.resize(_MAX_FRAMES_IN_FLIGHT);
		_f_inFlight.resize(_MAX_FRAMES_IN_FLIGHT);
		_f_imagesInFlight.resize(_swapChainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < _MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(_vkContext.logicalDevice, &semaphoreInfo, nullptr, &_s_imageAvailable[i]) != VK_SUCCESS)
				RARE_FATAL("Failed to create imageAvailable semaphore");
			if (vkCreateSemaphore(_vkContext.logicalDevice, &semaphoreInfo, nullptr, &_s_renderFinished[i]) != VK_SUCCESS)
				RARE_FATAL("Failed to create renderFinished semaphore");
			if (vkCreateFence(_vkContext.logicalDevice, &fenceInfo, nullptr, &_f_inFlight[i]) != VK_SUCCESS)
				RARE_FATAL("Failed to create inFlight fence");

		}
	}

	void RareCore::_populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = _debugCallback;
		createInfo.pUserData = nullptr;
	}

	void RareCore::_setupDebugMessenger() {
		if (!_enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		_populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(_vkContext.vkInstance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS) {
			RARE_FATAL("failed to set up debug messenger");
		}
		RARE_DEBUG("Created debug messenger");
	}

	bool RareCore::_checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		RARE_DEBUG("Available validation layers: {}", layerCount);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); //this populates availableLayers with the current usable layers, not the ones to be used

		//this goes thru the available layers to find the specific layers we specified we wanted in the constructor
		for (const char* vLayerName : _validationLayers) {
			bool layerFound = false;
			RARE_DEBUG("Checking for availability of '{}' layer", vLayerName);
			for (const auto& vLayerProperties : availableLayers) {
				if (strcmp(vLayerName, vLayerProperties.layerName) == 0) {
					layerFound = true;
					RARE_DEBUG("\tLayer '{}' available to be used.", vLayerName);
					break;
				}
			}

			if (!layerFound) {
				RARE_WARN("No layer available");
				return false;
			}
		}

		return true;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		} else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator) {

		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}


}
