#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>
#include <vma/vk_mem_alloc.h>

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <ranges>

const std::vector<char const*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

vk::raii::Context gVkRaiiContext;

vk::raii::Instance InitInstance()
{
	const auto appInfo = vk::ApplicationInfo{
		.pApplicationName   = "GCVulkan",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName        = "None",
		.engineVersion      = VK_MAKE_VERSION(0, 1, 0),
		.apiVersion         = vk::ApiVersion14
	};

	std::vector<char const*> requiredLayers;
	requiredLayers.assign(validationLayers.begin(), validationLayers.end());

	const auto layerProperties = gVkRaiiContext.enumerateInstanceLayerProperties();
	if (std::ranges::any_of(requiredLayers, [&layerProperties](auto const& requiredLayer) { return std::ranges::none_of(layerProperties, [requiredLayer](auto const& layerProperty) { return strcmp(layerProperty.layerName, requiredLayer) == 0; }); }))
		throw std::runtime_error("One or more required layers are not supported !");

	uint32_t glfwExtensionCount = 0;
	auto     glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	auto     extensionProperties = gVkRaiiContext.enumerateInstanceExtensionProperties();

	std::vector<const char*> activeExtensions;
	for (uint32_t i = 0; i < glfwExtensionCount; ++i)
		activeExtensions.push_back(glfwExtensions[i]);

	for (uint32_t i = 0; i < glfwExtensionCount; ++i)
		if (std::ranges::none_of(extensionProperties, [glfwExtension = glfwExtensions[i]](auto const& extensionProperty) { return strcmp(extensionProperty.extensionName, glfwExtension) == 0; }))
			throw std::runtime_error("Required GLFW extensions not supported");

	auto instanceCreateInfo = vk::InstanceCreateInfo{
		.pApplicationInfo        = &appInfo,
		.enabledLayerCount       = static_cast<uint32_t>(requiredLayers.size()),
		.ppEnabledLayerNames     = requiredLayers.data(),
		.enabledExtensionCount   = uint32_t(activeExtensions.size()),
		.ppEnabledExtensionNames = activeExtensions.data()
	};

	return vk::raii::Instance(gVkRaiiContext, instanceCreateInfo);
}

vk::raii::PhysicalDevice FetchPhysicalDevice(vk::raii::Instance& instance)
{
	auto devices = instance.enumeratePhysicalDevices();
	if (devices.empty())
		return nullptr;

	for (const auto& d : devices)
	{
		auto deviceProperties = d.getProperties();

		if (deviceProperties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu) continue;
		if (deviceProperties.apiVersion < VK_API_VERSION_1_4)                    continue;
		return d;
	}
}

uint32_t GetGraphicsQueueFamily(vk::raii::PhysicalDevice& physicalDevice)
{
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
	auto graphicsQueueFamilyProperty = std::find_if(
		queueFamilyProperties.begin(),
		queueFamilyProperties.end(),
		[](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }
	);

	return static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));
}

vk::raii::SurfaceKHR CreateGlfwWindowSurface(vk::raii::Instance& instance, GLFWwindow* window)
{
	VkSurfaceKHR _surface;
	if (glfwCreateWindowSurface(*instance, window, nullptr, &_surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create GLFW window surface");
	return vk::raii::SurfaceKHR(instance, _surface);
}

vk::raii::Device CreateDevice(vk::raii::PhysicalDevice& physicalDevice, uint32_t graphicsQueueFamily)
{
	float queuePriority = 0.0f;
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
	auto deviceQueueCreateInfo = vk::DeviceQueueCreateInfo {
		.queueFamilyIndex = graphicsQueueFamily,
		.queueCount       = 1,
		.pQueuePriorities = &queuePriority
	};

	vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features> featureChain = {
		{ .features = {.samplerAnisotropy = true } },
		{ .bufferDeviceAddress = true },
		{.synchronization2 = true, .dynamicRendering = true },
	};

	std::vector<const char*> deviceExtensions = {
		vk::KHRSwapchainExtensionName,
		vk::KHRSpirv14ExtensionName,
		vk::KHRSynchronization2ExtensionName
	};

	vk::DeviceCreateInfo deviceCreateInfo{
		.pNext                   = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
		.queueCreateInfoCount    = 1,
		.pQueueCreateInfos       = &deviceQueueCreateInfo,
		.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size()),
		.ppEnabledExtensionNames = deviceExtensions.data()
	};

	return vk::raii::Device(physicalDevice, deviceCreateInfo);
}

vk::raii::SwapchainKHR CreateSwapchain(vk::raii::PhysicalDevice& physicalDevice, vk::raii::Device& device, vk::raii::SurfaceKHR& surface, GLFWwindow* window)
{
	auto surfaceCaps           = physicalDevice.getSurfaceCapabilitiesKHR(surface);
	auto availableFormats      = physicalDevice.getSurfaceFormatsKHR(surface);
	auto availablePresentModes = physicalDevice.getSurfacePresentModesKHR(surface);

	auto swapSurfaceFormat = ([](const decltype(availableFormats)& formats) {
		for (const auto& availableFormat : formats)
			if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return availableFormat;
		return formats[0];
	})(availableFormats);

	auto swapExtent = ([&window](const vk::SurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		return vk::Extent2D {
			std::clamp<uint32_t>(width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width),
			std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
		};
	})(surfaceCaps);

	auto swapPresentMode = ([](const decltype(availablePresentModes)& modes) {
		for (const auto& availablePresentMode : modes)
			if (availablePresentMode == vk::PresentModeKHR::eFifoRelaxed)
				return availablePresentMode;
		return vk::PresentModeKHR::eFifo;
	})(availablePresentModes);

	auto minImageCount = std::clamp(3u, surfaceCaps.minImageCount, surfaceCaps.maxImageCount);

	auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR {
		.flags            = vk::SwapchainCreateFlagsKHR(0),
		.surface          = surface,
		.minImageCount    = minImageCount,
		.imageFormat      = swapSurfaceFormat.format,
		.imageColorSpace  = swapSurfaceFormat.colorSpace,
		.imageExtent      = swapExtent,
		.imageArrayLayers = 1,
		.imageUsage       = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
		.imageSharingMode = vk::SharingMode::eExclusive,
		.preTransform     = surfaceCaps.currentTransform,
		.compositeAlpha   = vk::CompositeAlphaFlagBitsKHR::eOpaque,
		.presentMode      = swapPresentMode,
		.clipped          = true,
		.oldSwapchain     = nullptr
	};

	return vk::raii::SwapchainKHR(device, swapchainCreateInfo);
}

struct FrameData
{
	vk::raii::Semaphore     imageAvailableSemaphore;
	vk::raii::Fence         inFlightFence;
	vk::raii::CommandBuffer commandBuffer = nullptr;
};

void step5()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	auto window = glfwCreateWindow(1280, 720, "Gaming Campus goes Vulkan", nullptr, nullptr);

	auto instance = InitInstance();
	auto physicalDevice = FetchPhysicalDevice(instance);
	if (physicalDevice == nullptr)
		throw std::runtime_error("Unable to fetch physical device");

	auto graphicsQueueFamily = GetGraphicsQueueFamily(physicalDevice);
	auto surface             = CreateGlfwWindowSurface(instance, window);
	auto device              = CreateDevice(physicalDevice, graphicsQueueFamily);
	auto graphicsQueue       = vk::raii::Queue(device, graphicsQueueFamily, 0);
	auto swapchain           = CreateSwapchain(physicalDevice, device, surface, window);
	auto swapchainImages     = swapchain.getImages();

	auto poolInfo = vk::CommandPoolCreateInfo {
		.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		.queueFamilyIndex = graphicsQueueFamily
	};
	auto commandPool = vk::raii::CommandPool(device, poolInfo);

	auto frameData = swapchainImages | std::views::transform([&](const vk::Image&) {
		return FrameData {
			.imageAvailableSemaphore = vk::raii::Semaphore(device, vk::SemaphoreCreateInfo {}),
			.inFlightFence           = vk::raii::Fence(device, vk::FenceCreateInfo {.flags = vk::FenceCreateFlagBits::eSignaled })
		};
	}) | std::ranges::to<std::vector>();

	for (auto& fd : frameData)
	{
		vk::CommandBufferAllocateInfo allocInfo {
			.commandPool        = commandPool,
			.level              = vk::CommandBufferLevel::ePrimary,
			.commandBufferCount = 1
		};

		fd.commandBuffer = std::move(vk::raii::CommandBuffers(device, allocInfo).front());
	}

	do
	{
		glfwPollEvents();
	} while (!glfwWindowShouldClose(window));

	device.waitIdle();

	frameData.clear();
	commandPool.clear();
	swapchainImages.clear();
	swapchain.clear();
	graphicsQueue.clear();
	device.clear();
	surface.clear();
	physicalDevice.clear();
	instance.clear();
	glfwDestroyWindow(window);
	glfwTerminate();
}
