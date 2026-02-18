#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>
#include <vma/vk_mem_alloc.h>

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

#ifndef NDEBUG
# pragma comment(lib, "glfw3-s-d.lib")
#else /* NDEBUG */
# pragma comment(lib, "glfw3-s.lib")
#endif /* NDEBUG */

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


const std::vector<char const*> validationLayers = 
{
	"VK_LAYER_KHRONOS_validation"
};



#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif



class HelloTriangleApplication
{

private:
	GLFWwindow* window								= nullptr;

private:
	vk::raii::Context context;
	vk::raii::Instance instance                     = nullptr;
	vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

private:
	vk::raii::PhysicalDevice physicalDevice         = nullptr;
	vk::raii::Device device                         = nullptr;

private:
	vk::raii::Queue queue							= nullptr;

private:
	vk::raii::SurfaceKHR surface					= nullptr;

private:
	vk::raii::SwapchainKHR swapchain				= nullptr;
	vk::Format swapChainImageFormat					= vk::Format::eUndefined;
	vk::Extent2D swapChainExtent;

private:
	std::vector<const char*> requiredDeviceExtension =
	{
		vk::KHRSwapchainExtensionName
	};

private:
	std::vector<vk::Image> swapChainImages;

private:
	std::vector<vk::raii::ImageView> swapChainImageViews;



	



private:
	
	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Tuto", nullptr, nullptr);

	}
	std::vector<const char*>getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers)
			extensions.push_back(vk::EXTDebugUtilsExtensionName);

		return extensions;
	}
	void createInstance()

	{
		/*ApplicationInfo sert à envoyer les paramètres de l'application,
		 *pour que le driver Vulkan puisse debug
		 *ou fournir des statistiques sur l'application*/
		constexpr vk::ApplicationInfo appInfo
		{
			.pApplicationName = "Hello Triangle",
			.applicationVersion = VK_MAKE_VERSION(1,0,0),
			.pEngineName = "No Engine",
			.engineVersion = VK_MAKE_VERSION(1,0,0),
			.apiVersion = vk::ApiVersion14,
		};

		// Get the required layers
		std::vector<char const*> requiredLayers;
		if (enableValidationLayers)
			requiredLayers.assign(validationLayers.begin(), validationLayers.end());

		// Check if the required layers are supported by the Vulkan implementation.
		auto layerProperties = context.enumerateInstanceLayerProperties();
		for (auto const& requiredLayers : requiredLayers)
		{
			if (std::ranges::none_of
			(layerProperties, [requiredLayers](auto const& layerProperty)
				{
					return strcmp(layerProperty.layerName, requiredLayers) == 0;
				}))
			{
				throw std::runtime_error("Required layer not supported : " + std::string(requiredLayers));
			}
		}
		
		// Get the required extensions.
		auto requiredExtensions = getRequiredExtensions();

		// Check if the required extensions are supported by the Vulkan implementation.
		auto extensionProperties = context.enumerateInstanceExtensionProperties();
		for (auto const& requiredExtension : requiredExtensions)
		{
			if (std::ranges::none_of
			(extensionProperties, [requiredExtension](auto const& extensionProperty)
				{
					return strcmp(extensionProperty.extensionName, requiredExtension) == 0;
				}))
			{
				throw std::runtime_error("Required extension not supported : " + std::string(requiredExtension));
			}
		}


		//Info pour créer l'instance
		vk::InstanceCreateInfo createInfo
		{
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
			.ppEnabledLayerNames = requiredLayers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
			.ppEnabledExtensionNames = requiredExtensions.data()
		};

		instance = vk::raii::Instance(context, createInfo);
	}
	static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback
	(
		vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
		vk::DebugUtilsMessageTypeFlagsEXT type,
		const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*)
	{
		if (severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError || severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
		{
			std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;
		}

		return vk::False;
	}
	void setupDebugMessenger()
	{
		if (!enableValidationLayers)
			return;


		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
															vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
															vk::DebugUtilsMessageSeverityFlagBitsEXT::eError   );
		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral     |
														    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
														    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation  );
		vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT
		{
			.messageSeverity = severityFlags,
			.messageType = messageTypeFlags,
			.pfnUserCallback = &debugCallback
		};
		debugMessenger = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
	}
	void createSurface()
	{
		VkSurfaceKHR _surface;
		if (glfwCreateWindowSurface(*instance, window, nullptr, &_surface) != 0)
			throw std::runtime_error("Failed to create Window Surface ! ");
		surface = vk::raii::SurfaceKHR(instance, _surface);
	}
	void pickPhysicalDevice()
	{
		auto devices = instance.enumeratePhysicalDevices();
		
		const auto devIter = 
			std::ranges::find_if(devices, [&](auto const& device)
			{
				// Check if the device supports the Vulkan 1.3 API version
				bool supportsVulkan1_3 = device.getProperties().apiVersion >= VK_API_VERSION_1_3;

				// Check if any of the queue families support graphics operations
				auto queueFamilies = device.getQueueFamilyProperties();
				bool supportGraphics = 
					std::ranges::any_of(queueFamilies, [](vk::QueueFamilyProperties const& qfp)
				{
					return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics);
				});
				// Check if all required device extensions are available
				auto availableDeviceExtensions = device.enumerateDeviceExtensionProperties();
				bool supportsAllRequiredExtensions =
					std::ranges::all_of(requiredDeviceExtension, [&availableDeviceExtensions](auto const& requiredDeviceExtension)
						{
							return std::ranges::any_of(availableDeviceExtensions, [requiredDeviceExtension](auto const& availableDeviceExtension)
								{return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0; });
						});
				auto features = 
					device.template getFeatures2
				<vk::PhysicalDeviceFeatures2, 
				vk::PhysicalDeviceVulkan13Features, 
				vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
				bool supportsRequiredFeatures =
					features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
					features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;
				return supportsVulkan1_3 && supportGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
			});
		if (devIter != devices.end())
			physicalDevice = *devIter;
		else
			throw std::runtime_error("Failed to find a suitable GPU !!!");
			
	}
	void createLogicalDevice()
	{
		// find the index of the first queue family that supports graphics
		std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
		
		//get the first index into queueFamilyProperties which supports graphics
		uint32_t queueIndex = ~0;
		for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++)
		{
			if ((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
				physicalDevice.getSurfaceSupportKHR(qfpIndex, *surface))
			{
				// found a queue family that supports both graphics and present
				queueIndex = qfpIndex;
				break;
			}
		}
		if (queueIndex == ~0)
			throw std::runtime_error(" Could not find a queue for graphics and present -> terminating");

		//query for Vulkan 1.3 features
		vk::StructureChain
			<vk::PhysicalDeviceFeatures2,
			vk::PhysicalDeviceVulkan13Features,
			vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain =
		{
		{},								//vk::PhysicalDeviceFeatures2
		{.dynamicRendering = true},	    //vk::PhysicalDeviceVulkan13Features
		{.extendedDynamicState = true}	//vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
		};

		//create a Device
		float QueuePriority = 0.5f;
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo
		{ 
			.queueFamilyIndex = queueIndex, 
			.queueCount		  = 1, 
			.pQueuePriorities = &QueuePriority 
		};
		vk::DeviceCreateInfo deviceCreateInfo
		{
			.pNext					 = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
			.queueCreateInfoCount    = 1,
			.pQueueCreateInfos		 = &deviceQueueCreateInfo,
			.enabledExtensionCount   = static_cast<uint32_t>(requiredDeviceExtension.size()),
			.ppEnabledExtensionNames = requiredDeviceExtension.data()
		};

		device = vk::raii::Device(physicalDevice, deviceCreateInfo);
		queue = vk::raii::Queue(device, queueIndex, 0);
	}
	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats)
	{
		assert(!availableFormats.empty());
		for (const auto &availableFormat : availableFormats)
		{
			if (availableFormat.format     == vk::Format::eB8G8R8A8Srgb       &&
			    availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return availableFormat;
		}
		return  availableFormats[0];
	}
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes)
	{
		assert(std::ranges::any_of
		(availablePresentModes, [](auto presentMode)
		{
			return presentMode == vk::PresentModeKHR::eFifo;
		}));
		return std::ranges::any_of
		(availablePresentModes,[](const vk::PresentModeKHR value)
		{
			return vk::PresentModeKHR::eMailbox == value;
		}) ?
			vk::PresentModeKHR::eMailbox :
			vk::PresentModeKHR::eFifo;
	}
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		return
		{
			std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
			std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
		};
	}
	void createSwapChain()
	{
		auto _surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		auto _swapChainSurfaceFormat = chooseSwapSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(*surface));
		auto _swapChainExtent = chooseSwapExtent(_surfaceCapabilities);
		auto _minImageCount = std::max(3u, _surfaceCapabilities.minImageCount);
		_minImageCount = (_surfaceCapabilities.maxImageCount > 0 &&
						 _minImageCount > _surfaceCapabilities.maxImageCount) ? _surfaceCapabilities.maxImageCount : _minImageCount;
		
		vk::SwapchainCreateInfoKHR swapChainCreateInfo
		{
			.flags = vk::SwapchainCreateFlagsKHR(),
			.surface = *surface,
			.minImageCount = _minImageCount,
			.imageFormat = _swapChainSurfaceFormat.format,
			.imageColorSpace = _swapChainSurfaceFormat.colorSpace,
			.imageExtent = _swapChainExtent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode = vk::SharingMode::eExclusive,
			.preTransform = _surfaceCapabilities.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode = chooseSwapPresentMode(physicalDevice.getSurfacePresentModesKHR(*surface)),
			.clipped = true,
			.oldSwapchain = nullptr
		};
		
		swapchain = vk::raii::SwapchainKHR(device, swapChainCreateInfo);
		swapChainImages = swapchain.getImages();
		swapChainImageFormat = _swapChainSurfaceFormat.format;
		swapChainExtent = _swapChainExtent;
	}
	void createImageViews()
	{
		swapChainImageViews.clear();

		vk::ImageViewCreateInfo imageViewCreateInfo
		{
			.viewType = vk::ImageViewType::e2D,
			.format = swapChainImageFormat,
			.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
		};

		for (auto &image : swapChainImages)
		{
			imageViewCreateInfo.image = image;
			swapChainImageViews.emplace_back(device, imageViewCreateInfo);
		}
	}
	static std::vector<char> readFile(const std::string &filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			throw std::runtime_error("Failed to open file !");

		std::vector<char> buffer(file.tellg());

		file.seekg(0, std::ios::beg);
		file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
		file.close();

		return buffer;
	}
	[[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char> &code)
	{
		vk::ShaderModuleCreateInfo createInfo
		{
			.codeSize = code.size() * sizeof(char),
			.pCode = reinterpret_cast<const uint32_t*>(code.data())
		};
		vk::raii::ShaderModule shaderModule{ device, createInfo };
		return shaderModule;
	}
	void createGraphicsPipeline()
	{
		vk::raii::ShaderModule shaderModule = createShaderModule(readFile("shaders/slang.spv"));

		vk::PipelineShaderStageCreateInfo vertShaderStageInfo
		{
			.stage = vk::ShaderStageFlagBits::eVertex,
			.module = shaderModule,
			.pName = "vertMain",
			.pSpecializationInfo = nullptr
		};

		vk::PipelineShaderStageCreateInfo fragShaderStageInfo
		{
			.stage = vk::ShaderStageFlagBits::eFragment,
			.module = shaderModule,
			.pName = "fragMain",
			.pSpecializationInfo = nullptr
		};
		
		vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
	}
	void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createGraphicsPipeline();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		//instance.clear();
		glfwDestroyWindow(window);

		glfwTerminate();
	}
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
};
int main()
{
	HelloTriangleApplication app;

	try
	{
		app.run();
	} catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}