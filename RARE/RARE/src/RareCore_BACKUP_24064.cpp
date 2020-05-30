#include "RareCore.h"

namespace Rare {

	RareCore::RareCore():_windowRefName("Default Name") { _coreShouldClose = false; }
	RareCore::RareCore(const char* windowName) : _windowRefName(windowName) { _coreShouldClose = false; }

	void RareCore::init() {

		//begin logger initialization
		Rare::Logger::init();
		RARE_LOG("Logger:\t Initialization complete");

		//begin glfw initialization
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		_windowRef = glfwCreateWindow(640, 480, _windowRefName, NULL, NULL);
		glfwSetWindowPos(_windowRef, 640 * 2, 480);
		glfwMakeContextCurrent(_windowRef);
		RARE_LOG("GLFW:\t\t Initialization complete");

		//begin vk initialization
		_createVkInstance();
		

		RARE_LOG("Initialization complete");
	}

	void RareCore::_createVkInstance() {
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Core";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "RARE";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		uint32_t glfwExtensionCount = 0; const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);



		VkInstanceCreateInfo createInfo{};//tells vulkan driver which global extensions and validation layers to use
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		//nullptr cuz no custom allocator rn
		if (vkCreateInstance(&createInfo, nullptr, &_vkInstance) != VK_SUCCESS)
			RARE_FATAL("Failed to create vkInstance");

		_pickPhysicalDevice();
		if (_physicalDevice == VK_NULL_HANDLE) RARE_FATAL("Couldn't find a physical device");

		
		RARE_DEBUG("{} extension(s) supported", extensionCount);
		RARE_DEBUG("{} glfw extension(s) required", glfwExtensionCount);

		RARE_LOG("VulkanInit: Complete");

	}




	void RareCore::update() {
		static double start = glfwGetTime();
		static double delta;
		glfwPollEvents();//assign this to a daemon thread and lock event manager to synch assignments
<<<<<<< HEAD
		_coreShouldClose = (delta = glfwGetTime() - start) >= 12 ? true : false;
=======
		_coreShouldClose = (delta = glfwGetTime() - start) >= 45 ? true : false;
>>>>>>> 3f36690809660d64833950a4b627cc98905d2cac

	}
	void RareCore::render() {

	}

	void RareCore::dispose() {
		vkDestroyInstance(_vkInstance, nullptr);

		glfwDestroyWindow(_windowRef);
		glfwTerminate();
	
		RARE_LOG("{} Window closed", _windowRefName);
	}



	bool RareCore::_isDeviceSuitable(VkPhysicalDevice device) {
		

		// Use an ordered map to automatically sort candidates by
		//reasing score
		
		return true;
	}

	void RareCore::_pickPhysicalDevice() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);
		if (deviceCount == 0) RARE_FATAL("No PhysicalDevicesFound");
		RARE_DEBUG("{} physical device(s) found", deviceCount);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, devices.data());

		std::multimap<int, VkPhysicalDevice> candidates;

		auto rateDeviceSuitability = [](VkPhysicalDevice device)->int {
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
			auto deviceName = deviceProperties.deviceName;
			int score = 0;

			// Discrete GPUs have a significant performance advantage
			if (deviceProperties.deviceType ==
				VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				score += 1000;

			}

			score += deviceProperties.limits.maxImageDimension2D;

			if (!deviceFeatures.geometryShader) {
				RARE_ERROR("Device '{}' doesn't support geometry shaders", deviceName);//bad but not fatal
				score = 0;
			}
			if (!deviceFeatures.tessellationShader) {
				RARE_ERROR("Device '{}' doesn't support tesselation shaders", deviceName);
				score = 0;
			}
			if (!deviceFeatures.multiViewport) {
				RARE_WARN("Device '{}' doesn't support multiple viewports ", deviceName);
			}
			return score;
		};
		for (const auto& device : devices) {
			int score = rateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));

		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0) {
			_physicalDevice = candidates.rbegin()->second;
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(_physicalDevice, &deviceProperties);
			RARE_DEBUG("PhysicalDevice assigned to '{}'", deviceProperties.deviceName);
		} else {
			RARE_FATAL("failed to find a suitable GPU!");

		}
		

	}
}
