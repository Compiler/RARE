#include "CmdBufferFactory.h"
#include <Tools/Logging/Logger.h>
namespace Rare {
	VkCommandBuffer CmdBufferFactory::beginOneoffCommands(VulkanContext& context) {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = context.commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer cb;
		if (vkAllocateCommandBuffers(context.logicalDevice, &allocInfo, &cb) != VK_SUCCESS) {
			RARE_FATAL("Could not allocate one-off command buffer");
		}
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(cb, &beginInfo);
		return cb;
	}

	void CmdBufferFactory::endOneoffCommands(VulkanContext& context, VkCommandBuffer cb) {
		vkEndCommandBuffer(cb);
		VkSubmitInfo sinfo{};
		sinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		sinfo.commandBufferCount = 1;
		sinfo.pCommandBuffers = &cb;
		vkQueueSubmit(context.graphicsQueue, 1, &sinfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(context.graphicsQueue);
		/*
		There are again two possible ways to wait on this transfer to complete. We could use a fence and wait with vkWaitForFences,
		or simply wait for the transfer queue to become idle with vkQueueWaitIdle.
		A fence would allow you to schedule multiple transfers simultaneously and wait for all of them complete,
		instead of executing one at a time. That may give the driver more opportunities to optimize.
		*/

		vkFreeCommandBuffers(context.logicalDevice, context.commandPool, 1, &cb);
	}
}