#pragma once
#include "RareCommon.h"
#include "RareStructures.h"

namespace Rare {

	class CmdBufferFactory {
	public:
		CmdBufferFactory() = delete;

		
		static VkCommandBuffer beginOneoffCommands(VulkanContext& context);
		static void endOneoffCommands(VulkanContext& context, VkCommandBuffer cb);
	};
}