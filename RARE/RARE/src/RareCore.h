#pragma once

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <tools/Logger.h>


namespace Rare {
	class RareCore {

	private:
		//TODO: Abstract 'window' into its own class
		GLFWwindow* _windowRef;
		const char* _windowRefName;

		bool _coreShouldClose;
	public:
		RareCore();
		RareCore(const char* windowName);
		void init();
		void update();
		void render();
		void dispose();

		inline bool shouldClose() const { return _coreShouldClose; }


	};

}
