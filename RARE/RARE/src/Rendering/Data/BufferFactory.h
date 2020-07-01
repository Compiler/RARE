#pragma once
#include "RareCommon.h"
#include <RareStructures.h>

namespace Rare {

	class BufferFactory {
	public:
		static uint32_t findMemoryType(VulkanContext& context, uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static void createBuffer(VulkanContext& context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void copyBuffer(VulkanContext& context, VkBuffer src, VkBuffer dst, VkDeviceSize size);
		static void createVertexBuffer(VulkanContext& context, std::vector<VertexData>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
		static void createIndexBuffer(VulkanContext& context, std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory);
	};

}