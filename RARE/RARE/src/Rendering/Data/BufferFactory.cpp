#include "RareCommon.h"
#include <RareStructures.h>
#include "BufferFactory.h"
#include <Tools/Logging/Logger.h>
#include "Rendering/Commands/CmdBufferFactory.h"
namespace Rare {
	void BufferFactory::createBuffer(VulkanContext& context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.flags = 0;
		if (vkCreateBuffer(context.logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			RARE_FATAL("failed to create buffer");
		}


		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(context.logicalDevice, buffer, &memRequirements);
		RARE_DEBUG("Needs {} bytes for buffer", memRequirements.size);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(context, memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(context.logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			RARE_FATAL("failed to allocate buffer memory");
		}
		vkBindBufferMemory(context.logicalDevice, buffer, bufferMemory, 0);

	}

	void BufferFactory::createIndexBuffer(VulkanContext& context, std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory ) {
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);//staging buffer app side


		void* data;
		vkMapMemory(context.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(context.logicalDevice, stagingBufferMemory);

		createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);//Index buffer on gpu

		copyBuffer(context, stagingBuffer, indexBuffer, bufferSize);

		vkDestroyBuffer(context.logicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(context.logicalDevice, stagingBufferMemory, nullptr);
	}

	void BufferFactory::createVertexBuffer(VulkanContext& context, std::vector<VertexData>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory) {
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);//staging buffer app side


		void* data;
		vkMapMemory(context.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(context.logicalDevice, stagingBufferMemory);

		createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);//vertex buffer on gpu

		copyBuffer(context, stagingBuffer, vertexBuffer, bufferSize);

		vkDestroyBuffer(context.logicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(context.logicalDevice, stagingBufferMemory, nullptr);
	}

	void BufferFactory::copyBuffer(VulkanContext& context, VkBuffer src, VkBuffer dst, VkDeviceSize size) {
		/*
		You may wish to create a separate command pool for these kinds of short-lived buffers,
		because the implementation may be able to apply memory allocation optimizations.
		You should use the VK_COMMAND_POOL_CREATE_TRANSIENT_BIT(tmp) flag during command pool generation in that case.
		*/
		VkCommandBuffer cbuff = CmdBufferFactory::beginOneoffCommands(context);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(cbuff, src, dst, 1, &copyRegion);

		CmdBufferFactory::endOneoffCommands(context, cbuff);

	}

	uint32_t BufferFactory::findMemoryType(VulkanContext& context, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(context.physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				RARE_DEBUG("Using memory type '{}'", i);
				return i;
			}
		}
		RARE_FATAL("Couldn't find an applicable memory type");
	}
}