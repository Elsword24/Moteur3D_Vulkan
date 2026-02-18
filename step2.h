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

void step2()
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
	auto surface = CreateGlfwWindowSurface(instance, window);

	do
	{
		glfwPollEvents();
	} while (!glfwWindowShouldClose(window));

	surface.clear();
	physicalDevice.clear();
	instance.clear();
	glfwDestroyWindow(window);
	glfwTerminate();
}
