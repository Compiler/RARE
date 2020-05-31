#include "RareCore.h"

namespace Rare {
	struct RareCore::QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentationFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentationFamily.has_value();
		}
	};


	RareCore::RareCore():_windowRefName("Default Name"),  _validationLayers({"VK_LAYER_KHRONOS_validation"}){ _coreShouldClose = false; }
	RareCore::RareCore(const char* windowName) : _windowRefName(windowName), _validationLayers({ "VK_LAYER_KHRONOS_validation" }) { _coreShouldClose = false; }
	RareCore::RareCore(const char* windowName, const std::vector<const char*> validationLayers) 
		: _windowRefName(windowName), _validationLayers(validationLayers){ _coreShouldClose = false; }

	void RareCore::init() {

		//begin logger initialization
		Rare::Logger::init();
		
		RARE_LOG("Logger:\t\t Initialization complete");

		//begin glfw initialization
		RARE_LOG("GLFW:\t\t\t Begin init");
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		_windowRef = glfwCreateWindow(640, 480, _windowRefName, NULL, NULL);
		glfwSetWindowPos(_windowRef, 640 * 2, 480);
		glfwMakeContextCurrent(_windowRef);
		RARE_LOG("GLFW:\t\t\t Initialization complete\n");

		//begin vk initialization
		RARE_LOG("Vulkan:\t\t Begin init");
		_createVkInstance();
		RARE_LOG("Vulkan:\t\t Initialization complete\n");

		//begin vk validation layers initialization
		RARE_LOG("Validation Layers:\t Begin init");
		_setupDebugMessenger();
		RARE_LOG("Validation Layers:\t Initialization complete\n");


		//begin creating surface device
		RARE_LOG("Create Surface:\t Begin init");
		_createSurface();
		RARE_LOG("Create Surface:\t Initialization complete\n");

		//begin picking physical device
		RARE_LOG("Pick Physical Device:\t Begin init");
		_pickPhysicalDevice();
		RARE_LOG("Pick Physical Device:\t Initialization complete\n");


		//begin creating logical device
		RARE_LOG("Create Logical Device:\t Begin init");
		_createLogicalDevice();
		RARE_LOG("Create Logical Device:\t Initialization complete\n");



		RARE_LOG("Initialization complete");
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
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		//Validation Layer stuff
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (_enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
			createInfo.ppEnabledLayerNames = _validationLayers.data();

			_populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}


		//nullptr cuz no custom allocator rn
		if (vkCreateInstance(&createInfo, nullptr, &_vkInstance) != VK_SUCCESS)
			RARE_FATAL("Failed to create vkInstance");

		

		
		RARE_DEBUG("{} extension(s) supported", vkExtensionCount);
		RARE_DEBUG("{} glfw extension(s) required", static_cast<uint32_t>(glfwExtensions.size()));

	}

	void RareCore::_createSurface() {
		if (glfwCreateWindowSurface(_vkInstance, _windowRef, nullptr, &_surface) != VK_SUCCESS) {
			RARE_FATAL("Couldn't create window surface");
		}
	}

	void RareCore::_createLogicalDevice() {
		QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);//find queue families for graphics only

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
		createInfo.enabledExtensionCount = 0;

		if (_enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
			createInfo.ppEnabledLayerNames = _validationLayers.data();
		} else
			createInfo.enabledLayerCount = 0;

		if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_logicalDevice) != VK_SUCCESS) RARE_FATAL("Couldn't create logical device");

		vkGetDeviceQueue(_logicalDevice, indices.presentationFamily.value(), 0, &_presentationQueue);//index 0 cuz one queue family (graphics)
		vkGetDeviceQueue(_logicalDevice, indices.graphicsFamily.value(), 0, &_graphicsQueue);

	}

	void RareCore::update() {
		static double start = glfwGetTime();
		static double delta;
		glfwPollEvents();//assign this to a daemon thread and lock event manager to synch assignments
		_coreShouldClose = (delta = glfwGetTime() - start) >= 5 ? true : false;

	}

	void RareCore::render() {

	}

	void RareCore::dispose() {
		vkDestroyDevice(_logicalDevice, nullptr);
		vkDestroySurfaceKHR(_vkInstance, _surface, nullptr);
		if (_enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(_vkInstance, _debugMessenger, nullptr);
		}

		vkDestroyInstance(_vkInstance, nullptr);

		glfwDestroyWindow(_windowRef);
		glfwTerminate();
	
		RARE_LOG("{} Window closed", _windowRefName);
	}

	bool RareCore::_isDeviceSuitable(VkPhysicalDevice device) {
		QueueFamilyIndices indices = findQueueFamilies(device);
		return indices.isComplete();
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

		if (_physicalDevice == VK_NULL_HANDLE) RARE_FATAL("Couldn't find a physical device");

		

	}

	RareCore::QueueFamilyIndices RareCore::findQueueFamilies(VkPhysicalDevice device) {
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
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, index, _surface, &presentSupport);
			if (presentSupport) indices.presentationFamily = index;
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

		if (CreateDebugUtilsMessengerEXT(_vkInstance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS) {
			RARE_FATAL("failed to set up debug messenger");
		}
		RARE_DEBUG("Created debug messenger");
	}
	
	bool RareCore::_checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* vLayerName : _validationLayers) {
			bool layerFound = false;
			RARE_DEBUG("Checking '{}' validation layer", vLayerName);
			for (const auto& vLayerProperties : availableLayers) {
				if (strcmp(vLayerName, vLayerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
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
