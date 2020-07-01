#pragma once
#include "RareCommon.h"

namespace Rare {
	struct VulkanContext {
		VkInstance vkInstance;
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
		VkCommandPool commandPool;
		VkQueue graphicsQueue;
	};
	template<typename T> struct Optional {
		T value;
		bool has_value;
	};
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct VertexData {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VertexData);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(VertexData, position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(VertexData, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(VertexData, texCoord);

			return attributeDescriptions;
		}
	};
	struct  UniformBufferObject {
		alignas(16) float time;
		alignas(16)glm::mat4 model;
		alignas(16)glm::mat4 view;
		alignas(16)glm::mat4 proj;
	};
}