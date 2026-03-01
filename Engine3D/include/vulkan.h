#ifndef VULKAN_H
#define VULKAN_H
#pragma once

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

//#include "mesh.h"

//struct Vertex
//{
//	glm::vec3 pos;
//	glm::vec3 normal;
//	glm::vec2 uv;
//
//	static vk::VertexInputBindingDescription getBindingDescription()
//	{
//		return { 0, sizeof(Vertex), vk::VertexInputRate::eVertex };
//	}
//
//	static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions()
//	{
//		return {
//			vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)),
//			vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal)),
//			vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, uv)) };
//	}
//
//};
//
//struct UniformBufferObject
//{
//	glm::mat4 model;
//	glm::mat4 view;
//	glm::mat4 proj;
//};

//std::vector<Vertex> vertices;
//std::vector<uint32_t> indices;

//Mesh LoadModel(const std::string& filePath)
//{
//	Mesh mesh;
//	mesh.vertices.clear();
//	mesh.indices.clear();
//
//	auto rapidObj = rapidobj::ParseFile(filePath);
//
//	if (!std::filesystem::exists(filePath))
//	{
//		throw std::runtime_error("model file not found! " + std::filesystem::absolute(filePath).string());
//	}
//
//	if (rapidObj.error)
//	{
//		throw std::runtime_error("Failed to load model! " + rapidObj.error.line);
//	}
//
//	rapidobj::Triangulate(rapidObj);
//
//	for (const auto& shape : rapidObj.shapes)
//	{
//		for (const auto& index : shape.mesh.indices)
//		{
//			Vertex vertex{};
//			vertex.pos =
//			{
//				rapidObj.attributes.positions[3 * index.position_index + 0],
//				rapidObj.attributes.positions[3 * index.position_index + 1],
//				rapidObj.attributes.positions[3 * index.position_index + 2]
//
//			};
//
//
//			if (index.normal_index != -1)
//			{
//				vertex.normal =
//				{
//					rapidObj.attributes.normals[3 * index.normal_index + 0],
//					rapidObj.attributes.normals[3 * index.normal_index + 1],
//					rapidObj.attributes.normals[3 * index.normal_index + 2],
//				};
//			}
//			else
//			{
//				vertex.normal = { 0.0f, 1.0f, 0.0f };
//			};
//
//			if (index.texcoord_index != -1)
//			{
//				vertex.uv =
//				{
//					rapidObj.attributes.texcoords[2 * index.texcoord_index + 0],
//					1.0f - rapidObj.attributes.texcoords[2 * index.texcoord_index + 1]
//
//				};
//			}
//			else
//			{
//				vertex.uv = { 0.0f, 0.0f };
//			};
//
//			mesh.indices.push_back(static_cast<uint32_t>(mesh.indices.size()));
//			mesh.vertices.push_back(vertex);
//		}
//	}
//
//	std::cout << "Loaded model with " << mesh.vertices.size() << " vertices." << std::endl;
//	std::cout << "Loaded model with " << mesh.indices.size() << " indices." << std::endl;
//	return mesh;
//}


//class ThirdPersonCamera : public Camera
//{
//private:
//	glm::vec3 targetPosition;
//	glm::vec3 targetForward;
//	float followDistance;
//	float followHeight;
//	float followSmoothness;
//	float minDstance;
//	float raycastDistance;
//	glm::vec3 desiredPosition;
//	glm::vec3 smoothDampVelocity;
//
//public:
//
//	ThirdPersonCamera(
//		float followDistance = 5.0f,
//		float followHeight = 2.0f,
//		float followSmoothness = 0.1f,
//		float minDistance = 1.0f
//	);
//
//	void updatePosition(
//		const glm::vec3& targetPos,
//		const glm::vec3& targetFwd,
//		float deltaTime
//	)
//	{
//		targetPosition = targetPos;
//		targetForward = glm::normalize(targetFwd);
//		glm::vec3 offset = -targetForward * followDistance;
//		offset.y = followHeight;
//
//		desiredPosition = targetPosition + offset;
//
//		camera.SetPosition(glm::mix(camera.getPosition(), desiredPosition, 1.0f - pow(followSmoothness, deltaTime * 60.0f)));
//
//		camera.SetFront(glm::normalize(targetPosition - camera.getPosition()));
//
//		camera.SetRight(glm::normalize(glm::cross(camera.getFront(), camera.GetWordlUp())));
//		camera.SetUp(glm::normalize(glm::cross(camera.GetRight(), camera.getFront())));
//	}
//	//void handleOcclusion(const Scene& scene) {
//	// Cast a ray from the target to the desired camera position
//	//Ray ray;
//	//ray.origin = targetPosition;
//	//ray.direction = glm::normalize(desiredPosition - targetPosition);
//
//	//// Check for intersections with scene objects
//	//RaycastHit hit;
//	//if (scene.raycast(ray, hit, glm::length(desiredPosition - targetPosition))) {
//	//	// If there's an intersection, move the camera to the hit point
//	//	// minus a small offset to avoid clipping
//	//	float offsetDistance = 0.2f;
//	//	position = hit.point - (ray.direction * offsetDistance);
//
//	//	// Ensure we don't get too close to the target
//	//	float currentDistance = glm::length(position - targetPosition);
//	//	if (currentDistance < minDistance) {
//	//		position = targetPosition + ray.direction * minDistance;
//	//	}
//
//	//	// Update the camera to look at the target
//	//	front = glm::normalize(targetPosition - position);
//	//	right = glm::normalize(glm::cross(front, worldUp));
//	//	up = glm::normalize(glm::cross(right, front));
//	//}
//	void orbit(float horizontalAngle, float verticalAngle);
//
//	void setFollowDistance(float distance) { followDistance = distance; }
//	void setFollowHeight(float height) { followHeight = height; }
//	void setFollowSmoothness(float smoothness) { followSmoothness = smoothness; }
//};

//std::vector<Mesh> meshes;

class VulkanRAII
{
private:
	GLFWwindow* window = nullptr;

private:
	vk::raii::Context context;
	vk::raii::Instance instance = nullptr;
	vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

private:
	vk::raii::PhysicalDevice physicalDevice = nullptr;
	vk::raii::Device device = nullptr;

private:
	vk::raii::Queue queue = nullptr;

private:
	vk::raii::SurfaceKHR surface = nullptr;

private:
	vk::raii::SwapchainKHR swapchain = nullptr;
	vk::Format swapChainImageFormat = vk::Format::eUndefined;
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
	vk::raii::PipelineLayout pipelineLayout = nullptr;
	vk::raii::Pipeline		 graphicsPipeline = nullptr;

private:

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Engine 3D With Vulkan", nullptr, nullptr);

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
			.pApplicationName = "Engine 3D",
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
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
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
			vk::PhysicalDeviceVulkan11Features,
			vk::PhysicalDeviceVulkan13Features,
			vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain =
		{
		{},								//vk::PhysicalDeviceFeatures2
		{.shaderDrawParameters = true},  //vk::PhysicalDeviceVulkan11Features
		{.dynamicRendering = true},	    //vk::PhysicalDeviceVulkan13Features
		{.extendedDynamicState = true}	//vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
		};

		//create a Device
		float QueuePriority = 0.5f;
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo
		{
			.queueFamilyIndex = queueIndex,
			.queueCount = 1,
			.pQueuePriorities = &QueuePriority
		};
		vk::DeviceCreateInfo deviceCreateInfo
		{
			.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &deviceQueueCreateInfo,
			.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
			.ppEnabledExtensionNames = requiredDeviceExtension.data()
		};

		device = vk::raii::Device(physicalDevice, deviceCreateInfo);
		queue = vk::raii::Queue(device, queueIndex, 0);
	}
	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
	{
		assert(!availableFormats.empty());
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
				availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return availableFormat;
		}
		return  availableFormats[0];
	}
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
	{
		assert(std::ranges::any_of
		(availablePresentModes, [](auto presentMode)
			{
				return presentMode == vk::PresentModeKHR::eFifo;
			}));
		return std::ranges::any_of
		(availablePresentModes, [](const vk::PresentModeKHR value)
			{
				return vk::PresentModeKHR::eMailbox == value;
			}) ?
			vk::PresentModeKHR::eMailbox :
				vk::PresentModeKHR::eFifo;
	}
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
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

		for (auto& image : swapChainImages)
		{
			imageViewCreateInfo.image = image;
			swapChainImageViews.emplace_back(device, imageViewCreateInfo);
		}
	}
	static std::vector<char> readFile(const std::string& filename)
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
	[[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char>& code)
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
		vk::raii::ShaderModule shaderModule = createShaderModule(readFile("shaders/DrawTriangle.spv"));

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

		vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		vk::PipelineVertexInputStateCreateInfo   vertexInputInfo;

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly
		{ 
			.topology = vk::PrimitiveTopology::eTriangleList 
		};

		vk::PipelineViewportStateCreateInfo      viewportState
		{ 
			.viewportCount = 1,
			.scissorCount = 1 
		};

		vk::PipelineRasterizationStateCreateInfo rasterizer
		{ 
			.depthClampEnable = vk::False,
			.rasterizerDiscardEnable = vk::False,
			.polygonMode = vk::PolygonMode::eFill,
			.cullMode = vk::CullModeFlagBits::eBack,
			.frontFace = vk::FrontFace::eClockwise,
			.depthBiasEnable = vk::False,
			.depthBiasSlopeFactor = 1.0f,
			.lineWidth = 1.0f 
		};

		vk::PipelineMultisampleStateCreateInfo multisampling
		{ 
			.rasterizationSamples = vk::SampleCountFlagBits::e1,
			.sampleShadingEnable = vk::False 
		};

		vk::PipelineColorBlendAttachmentState colorBlendAttachment
		{ 
			.blendEnable = vk::False,
			.colorWriteMask = 
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA 
		};

		vk::PipelineColorBlendStateCreateInfo colorBlending
		{ 
			.logicOpEnable = vk::False,
			.logicOp = vk::LogicOp::eCopy,
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachment 
		};

		std::vector dynamicStates = 
		{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor 
		};

		vk::PipelineDynamicStateCreateInfo dynamicState
		{ 
			.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
			.pDynamicStates = dynamicStates.data() 
		};

		vk::PipelineLayoutCreateInfo pipelineLayoutInfo
		{ 
			.setLayoutCount = 0,
			.pushConstantRangeCount = 0 
		};

		pipelineLayout = vk::raii::PipelineLayout(device, pipelineLayoutInfo);

		vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = 
		{
			{
				.stageCount = 2,
				.pStages = shaderStages,
				.pVertexInputState = &vertexInputInfo,
				.pInputAssemblyState = &inputAssembly,
				 .pViewportState = &viewportState,
				 .pRasterizationState = &rasterizer,
				 .pMultisampleState = &multisampling,
				 .pColorBlendState = &colorBlending,
				 .pDynamicState = &dynamicState,
				 .layout = pipelineLayout,
				 .renderPass = nullptr
			},
			{
				.colorAttachmentCount = 1,
				.pColorAttachmentFormats = &swapChainImageFormat
			} 
		};

		graphicsPipeline = vk::raii::Pipeline(device, nullptr, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
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


//class HelloTriangleApplication
//{
//public:
//	HelloTriangleApplication()
//	{
//		window = new Window();
//		camera = std::make_unique<Camera>(window);
//
//		monkey = LoadModel("../shaders/monkey.obj");
//		box = LoadModel("../shaders/stormtrooper.obj");
//
//		meshes.push_back(std::move(box));
//		meshes.push_back(std::move(monkey));
//
//	}
//	~HelloTriangleApplication()
//	{
//		delete window;
//		window = nullptr;
//	}
//
//	void run()
//	{
//
//		initWindow();
//		initVulkan();
//		camera->setupInputCallbacks();
//		mainLoop();
//		cleanup();
//	}
//
//private:
//	Window* window = nullptr;
//	vk::raii::Context                context;
//	vk::raii::Instance               instance = nullptr;
//	vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
//	vk::raii::SurfaceKHR             surface = nullptr;
//	vk::raii::PhysicalDevice         physicalDevice = nullptr;
//	vk::raii::Device                 device = nullptr;
//	uint32_t                         queueIndex = ~0;
//	vk::raii::Queue                  queue = nullptr;
//	vk::raii::SwapchainKHR           swapChain = nullptr;
//	std::vector<vk::Image>           swapChainImages;
//	vk::SurfaceFormatKHR             swapChainSurfaceFormat;
//	vk::Extent2D                     swapChainExtent;
//	std::vector<vk::raii::ImageView> swapChainImageViews;
//
//	vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
//	vk::raii::PipelineLayout      pipelineLayout = nullptr;
//	vk::raii::Pipeline            graphicsPipeline = nullptr;
//
//	std::vector<vk::raii::Buffer>       uniformBuffers;
//	std::vector<vk::raii::DeviceMemory> uniformBuffersMemory;
//	std::vector<void*>                 uniformBuffersMapped;
//
//	vk::raii::DescriptorPool             descriptorPool = nullptr;
//	std::vector<vk::raii::DescriptorSet> descriptorSets;
//
//	vk::raii::CommandPool                commandPool = nullptr;
//	std::vector<vk::raii::CommandBuffer> commandBuffers;
//
//	std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
//	std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
//	std::vector<vk::raii::Fence>     inFlightFences;
//	uint32_t                         frameIndex = 0;
//	std::unique_ptr<Camera> camera;
//	Mesh monkey;
//	Mesh box;
//
//	bool framebufferResized = false;
//
//	std::vector<const char*> requiredDeviceExtension = {
//		vk::KHRSwapchainExtensionName };
//
//	void initWindow()
//	{
//		window->initWindow();
//	}
//
//	/*static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
//	{
//		auto app = static_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
//		app->framebufferResized = true;
//	}*/
//
//	void initVulkan()
//	{
//		createInstance();
//		setupDebugMessenger();
//		createSurface();
//		pickPhysicalDevice();
//		createLogicalDevice();
//		createSwapChain();
//		createImageViews();
//		createDescriptorSetLayout();
//		createGraphicsPipeline();
//		createTextureImage();
//		createCommandPool();
//		createVertexBuffer();
//		createIndexBuffer();
//		createUniformBuffers();
//		createDescriptorPool();
//		createDescriptorSets();
//		createCommandBuffers();
//		createSyncObjects();
//	}
//
//	void mainLoop()
//	{
//		while (!window->WindowClosed())
//		{
//			window->PollEvent();
//			camera->processInput(*window, *camera, 0.16f);
//			drawFrame();
//		}
//
//		device.waitIdle();
//	}
//
//	void cleanupSwapChain()
//	{
//		swapChainImageViews.clear();
//		swapChain = nullptr;
//	}
//
//	void cleanup()
//	{
//		window->cleanup();
//	}
//
//	void recreateSwapChain()
//	{
//		int width = 0, height = 0;
//		window->GetFramebufferSize(width, height);
//		while (width == 0 || height == 0)
//		{
//			window->GetFramebufferSize(width, height);
//			window->WaitEvents();
//		}
//
//		device.waitIdle();
//
//		cleanupSwapChain();
//		createSwapChain();
//		createImageViews();
//	}
//
//	void createInstance()
//	{
//		constexpr vk::ApplicationInfo appInfo{ .pApplicationName = "Hello Triangle",
//											  .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
//											  .pEngineName = "No Engine",
//											  .engineVersion = VK_MAKE_VERSION(1, 0, 0),
//											  .apiVersion = vk::ApiVersion14 };
//
//		// Get the required layers
//		std::vector<char const*> requiredLayers;
//		if (enableValidationLayers)
//		{
//			requiredLayers.assign(validationLayers.begin(), validationLayers.end());
//		}
//
//		// Check if the required layers are supported by the Vulkan implementation.
//		auto layerProperties = context.enumerateInstanceLayerProperties();
//		for (auto const& requiredLayer : requiredLayers)
//		{
//			if (std::ranges::none_of(layerProperties,
//				[requiredLayer](auto const& layerProperty) { return strcmp(layerProperty.layerName, requiredLayer) == 0; }))
//			{
//				throw std::runtime_error("Required layer not supported: " + std::string(requiredLayer));
//			}
//		}
//
//		// Get the required extensions.
//		auto requiredExtensions = getRequiredExtensions();
//
//		// Check if the required extensions are supported by the Vulkan implementation.
//		auto extensionProperties = context.enumerateInstanceExtensionProperties();
//		for (auto const& requiredExtension : requiredExtensions)
//		{
//			if (std::ranges::none_of(extensionProperties,
//				[requiredExtension](auto const& extensionProperty) { return strcmp(extensionProperty.extensionName, requiredExtension) == 0; }))
//			{
//				throw std::runtime_error("Required extension not supported: " + std::string(requiredExtension));
//			}
//		}
//
//		vk::InstanceCreateInfo createInfo{
//			.pApplicationInfo = &appInfo,
//			.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
//			.ppEnabledLayerNames = requiredLayers.data(),
//			.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
//			.ppEnabledExtensionNames = requiredExtensions.data() };
//		instance = vk::raii::Instance(context, createInfo);
//	}
//
//	void setupDebugMessenger()
//	{
//		if (!enableValidationLayers)
//			return;
//
//		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
//		vk::DebugUtilsMessageTypeFlagsEXT     messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
//		vk::DebugUtilsMessengerCreateInfoEXT  debugUtilsMessengerCreateInfoEXT{
//			 .messageSeverity = severityFlags,
//			 .messageType = messageTypeFlags,
//			 .pfnUserCallback = &debugCallback };
//		debugMessenger = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
//	}
//
//	void createSurface()
//	{
//		VkSurfaceKHR _surface;
//		if (glfwCreateWindowSurface(*instance, window->getGLFWWindow(), nullptr, &_surface) != 0)
//		{
//			throw std::runtime_error("failed to create window surface!");
//		}
//		surface = vk::raii::SurfaceKHR(instance, _surface);
//	}
//
//	void pickPhysicalDevice()
//	{
//		std::vector<vk::raii::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
//		const auto                            devIter = std::ranges::find_if(
//			devices,
//			[&](auto const& device) {
//				// Check if the device supports the Vulkan 1.3 API version
//				bool supportsVulkan1_3 = device.getProperties().apiVersion >= VK_API_VERSION_1_3;
//
//				// Check if any of the queue families support graphics operations
//				auto queueFamilies = device.getQueueFamilyProperties();
//				bool supportsGraphics =
//					std::ranges::any_of(queueFamilies, [](auto const& qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });
//
//				// Check if all required device extensions are available
//				auto availableDeviceExtensions = device.enumerateDeviceExtensionProperties();
//				bool supportsAllRequiredExtensions =
//					std::ranges::all_of(requiredDeviceExtension,
//						[&availableDeviceExtensions](auto const& requiredDeviceExtension) {
//							return std::ranges::any_of(availableDeviceExtensions,
//								[requiredDeviceExtension](auto const& availableDeviceExtension) { return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0; });
//						});
//
//				auto features = device.template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
//				bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
//					features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
//					features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;
//
//				return supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
//			});
//		if (devIter != devices.end())
//		{
//			physicalDevice = *devIter;
//		}
//		else
//		{
//			throw std::runtime_error("failed to find a suitable GPU!");
//		}
//	}
//
//	void createLogicalDevice()
//	{
//		std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
//
//		// get the first index into queueFamilyProperties which supports both graphics and present
//		for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++)
//		{
//			if ((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
//				physicalDevice.getSurfaceSupportKHR(qfpIndex, *surface))
//			{
//				// found a queue family that supports both graphics and present
//				queueIndex = qfpIndex;
//				break;
//			}
//		}
//		if (queueIndex == ~0)
//		{
//			throw std::runtime_error("Could not find a queue for graphics and present -> terminating");
//		}
//
//		// query for required features (Vulkan 1.1 and 1.3)
//		vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain = {
//			{},                                                          // vk::PhysicalDeviceFeatures2
//			{.shaderDrawParameters = true},                              // vk::PhysicalDeviceVulkan11Features
//			{.synchronization2 = true, .dynamicRendering = true},        // vk::PhysicalDeviceVulkan13Features
//			{.extendedDynamicState = true}                               // vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
//		};
//
//		// create a Device
//		float                     queuePriority = 0.5f;
//		vk::DeviceQueueCreateInfo deviceQueueCreateInfo{ .queueFamilyIndex = queueIndex, .queueCount = 1, .pQueuePriorities = &queuePriority };
//		vk::DeviceCreateInfo      deviceCreateInfo{ .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
//												   .queueCreateInfoCount = 1,
//												   .pQueueCreateInfos = &deviceQueueCreateInfo,
//												   .enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
//												   .ppEnabledExtensionNames = requiredDeviceExtension.data() };
//
//		device = vk::raii::Device(physicalDevice, deviceCreateInfo);
//		queue = vk::raii::Queue(device, queueIndex, 0);
//	}
//
//	void createSwapChain()
//	{
//		auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
//		swapChainExtent = chooseSwapExtent(surfaceCapabilities);
//		swapChainSurfaceFormat = chooseSwapSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(*surface));
//		vk::SwapchainCreateInfoKHR swapChainCreateInfo{ .surface = *surface,
//													   .minImageCount = chooseSwapMinImageCount(surfaceCapabilities),
//													   .imageFormat = swapChainSurfaceFormat.format,
//													   .imageColorSpace = swapChainSurfaceFormat.colorSpace,
//													   .imageExtent = swapChainExtent,
//													   .imageArrayLayers = 1,
//													   .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
//													   .imageSharingMode = vk::SharingMode::eExclusive,
//													   .preTransform = surfaceCapabilities.currentTransform,
//													   .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
//													   .presentMode = chooseSwapPresentMode(physicalDevice.getSurfacePresentModesKHR(*surface)),
//													   .clipped = true };
//
//		swapChain = vk::raii::SwapchainKHR(device, swapChainCreateInfo);
//		swapChainImages = swapChain.getImages();
//	}
//
//	void createImageViews()
//	{
//		assert(swapChainImageViews.empty());
//
//		vk::ImageViewCreateInfo imageViewCreateInfo{ .viewType = vk::ImageViewType::e2D, .format = swapChainSurfaceFormat.format, .subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1} };
//		for (auto& image : swapChainImages)
//		{
//			imageViewCreateInfo.image = image;
//			swapChainImageViews.emplace_back(device, imageViewCreateInfo);
//		}
//	}
//
//	void createDescriptorSetLayout()
//	{
//		vk::DescriptorSetLayoutBinding    uboLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr);
//		vk::DescriptorSetLayoutCreateInfo layoutInfo{ .bindingCount = 1, .pBindings = &uboLayoutBinding };
//		descriptorSetLayout = vk::raii::DescriptorSetLayout(device, layoutInfo);
//	}
//
//	void createGraphicsPipeline()
//	{
//		vk::raii::ShaderModule shaderModule = createShaderModule(readFile("../shaders/slang.spv"));
//
//		vk::PipelineShaderStageCreateInfo vertShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eVertex, .module = shaderModule, .pName = "vertMain" };
//		vk::PipelineShaderStageCreateInfo fragShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eFragment, .module = shaderModule, .pName = "fragMain" };
//		vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
//
//		auto                                     bindingDescription = Vertex::getBindingDescription();
//		auto                                     attributeDescriptions = Vertex::getAttributeDescriptions();
//		vk::PipelineVertexInputStateCreateInfo   vertexInputInfo{ .vertexBindingDescriptionCount = 1, .pVertexBindingDescriptions = &bindingDescription, .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()), .pVertexAttributeDescriptions = attributeDescriptions.data() };
//		vk::PipelineInputAssemblyStateCreateInfo inputAssembly{ .topology = vk::PrimitiveTopology::eTriangleList };
//		vk::PipelineViewportStateCreateInfo      viewportState{ .viewportCount = 1, .scissorCount = 1 };
//
//		vk::PipelineRasterizationStateCreateInfo rasterizer{ .depthClampEnable = vk::False, .rasterizerDiscardEnable = vk::False, .polygonMode = vk::PolygonMode::eFill, .cullMode = vk::CullModeFlagBits::eNone, .frontFace = vk::FrontFace::eCounterClockwise, .depthBiasEnable = vk::False, .depthBiasSlopeFactor = 1.0f, .lineWidth = 1.0f };
//
//		vk::PipelineMultisampleStateCreateInfo multisampling{ .rasterizationSamples = vk::SampleCountFlagBits::e1, .sampleShadingEnable = vk::False };
//
//		vk::PipelineColorBlendAttachmentState colorBlendAttachment{ .blendEnable = vk::False,
//																   .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };
//
//		vk::PipelineColorBlendStateCreateInfo colorBlending{ .logicOpEnable = vk::False, .logicOp = vk::LogicOp::eCopy, .attachmentCount = 1, .pAttachments = &colorBlendAttachment };
//
//		std::vector dynamicStates = {
//			vk::DynamicState::eViewport,
//			vk::DynamicState::eScissor };
//		vk::PipelineDynamicStateCreateInfo dynamicState{ .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()), .pDynamicStates = dynamicStates.data() };
//
//		vk::PushConstantRange pushConstant{ .stageFlags = vk::ShaderStageFlagBits::eVertex, .offset = 0, .size = sizeof(glm::mat4) };
//
//		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{ .setLayoutCount = 1, .pSetLayouts = &*descriptorSetLayout, .pushConstantRangeCount = 1, .pPushConstantRanges = &pushConstant };
//
//		pipelineLayout = vk::raii::PipelineLayout(device, pipelineLayoutInfo);
//
//		vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = {
//			{.stageCount = 2,
//			 .pStages = shaderStages,
//			 .pVertexInputState = &vertexInputInfo,
//			 .pInputAssemblyState = &inputAssembly,
//			 .pViewportState = &viewportState,
//			 .pRasterizationState = &rasterizer,
//			 .pMultisampleState = &multisampling,
//			 .pColorBlendState = &colorBlending,
//			 .pDynamicState = &dynamicState,
//			 .layout = pipelineLayout,
//			 .renderPass = nullptr},
//			{.colorAttachmentCount = 1, .pColorAttachmentFormats = &swapChainSurfaceFormat.format} };
//
//		graphicsPipeline = vk::raii::Pipeline(device, nullptr, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
//	}
//
//	void createTextureImage()
//	{
//
//	}
//
//	void createCommandPool()
//	{
//		vk::CommandPoolCreateInfo poolInfo{ .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
//										   .queueFamilyIndex = queueIndex };
//		commandPool = vk::raii::CommandPool(device, poolInfo);
//	}
//
//	void createVertexBuffer()
//	{
//		for (auto& mesh : meshes)
//		{
//			if (mesh.vertices.empty())
//			{
//				std::cerr << "vertices mesh is empty" << std::endl;
//			}
//
//			vk::DeviceSize         bufferSize = sizeof(mesh.vertices[0]) * mesh.vertices.size();
//			vk::raii::Buffer       stagingBuffer({});
//			vk::raii::DeviceMemory stagingBufferMemory({});
//
//			createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory); void* dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);
//			memcpy(dataStaging, mesh.vertices.data(), bufferSize);
//			stagingBufferMemory.unmapMemory();
//
//			createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, mesh.vertexBuffer, mesh.vertexBufferMemory);
//
//			copyBuffer(stagingBuffer, mesh.vertexBuffer, bufferSize);
//
//		}
//		if (meshes.empty())
//		{
//			std::cerr << "meshes is empty" << std::endl;
//		}
//
//
//	}
//
//	void createIndexBuffer()
//	{
//		for (auto& mesh : meshes)
//		{
//			vk::DeviceSize bufferSize = sizeof(mesh.indices[0]) * mesh.indices.size();
//
//			vk::raii::Buffer       stagingBuffer({});
//			vk::raii::DeviceMemory stagingBufferMemory({});
//			createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);
//
//			void* data = stagingBufferMemory.mapMemory(0, bufferSize);
//			memcpy(data, mesh.indices.data(), (size_t)bufferSize);
//			stagingBufferMemory.unmapMemory();
//
//			createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, mesh.indexBuffer, mesh.indexBufferMemory);
//
//			copyBuffer(stagingBuffer, mesh.indexBuffer, bufferSize);
//
//		}
//	}
//
//	void createUniformBuffers()
//	{
//		uniformBuffers.clear();
//		uniformBuffersMemory.clear();
//		uniformBuffersMapped.clear();
//
//		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//		{
//			vk::DeviceSize         bufferSize = sizeof(UniformBufferObject);
//			vk::raii::Buffer       buffer({});
//			vk::raii::DeviceMemory bufferMem({});
//			createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, buffer, bufferMem);
//			uniformBuffers.emplace_back(std::move(buffer));
//			uniformBuffersMemory.emplace_back(std::move(bufferMem));
//			uniformBuffersMapped.emplace_back(uniformBuffersMemory[i].mapMemory(0, bufferSize));
//		}
//	}
//
//	void createDescriptorPool()
//	{
//		vk::DescriptorPoolSize       poolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT);
//		vk::DescriptorPoolCreateInfo poolInfo{ .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, .maxSets = MAX_FRAMES_IN_FLIGHT, .poolSizeCount = 1, .pPoolSizes = &poolSize };
//		descriptorPool = vk::raii::DescriptorPool(device, poolInfo);
//	}
//
//	void createDescriptorSets()
//	{
//		std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout);
//		vk::DescriptorSetAllocateInfo        allocInfo{ .descriptorPool = descriptorPool, .descriptorSetCount = static_cast<uint32_t>(layouts.size()), .pSetLayouts = layouts.data() };
//
//		descriptorSets = device.allocateDescriptorSets(allocInfo);
//
//		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//		{
//			vk::DescriptorBufferInfo bufferInfo{ .buffer = uniformBuffers[i], .offset = 0, .range = sizeof(UniformBufferObject) };
//			vk::WriteDescriptorSet   descriptorWrite{ .dstSet = descriptorSets[i], .dstBinding = 0, .dstArrayElement = 0, .descriptorCount = 1, .descriptorType = vk::DescriptorType::eUniformBuffer, .pBufferInfo = &bufferInfo };
//			device.updateDescriptorSets(descriptorWrite, {});
//		}
//	}
//
//	void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory)
//	{
//		vk::BufferCreateInfo bufferInfo{ .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive };
//		buffer = vk::raii::Buffer(device, bufferInfo);
//		vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();
//		vk::MemoryAllocateInfo allocInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties) };
//		bufferMemory = vk::raii::DeviceMemory(device, allocInfo);
//		buffer.bindMemory(bufferMemory, 0);
//	}
//
//	void copyBuffer(vk::raii::Buffer& srcBuffer, vk::raii::Buffer& dstBuffer, vk::DeviceSize size)
//	{
//		vk::CommandBufferAllocateInfo allocInfo{ .commandPool = commandPool, .level = vk::CommandBufferLevel::ePrimary, .commandBufferCount = 1 };
//		vk::raii::CommandBuffer       commandCopyBuffer = std::move(device.allocateCommandBuffers(allocInfo).front());
//		commandCopyBuffer.begin(vk::CommandBufferBeginInfo{ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
//		commandCopyBuffer.copyBuffer(*srcBuffer, *dstBuffer, vk::BufferCopy(0, 0, size));
//		commandCopyBuffer.end();
//		queue.submit(vk::SubmitInfo{ .commandBufferCount = 1, .pCommandBuffers = &*commandCopyBuffer }, nullptr);
//		queue.waitIdle();
//	}
//
//	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
//	{
//		vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
//
//		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
//		{
//			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
//			{
//				return i;
//			}
//		}
//
//		throw std::runtime_error("failed to find suitable memory type!");
//	}
//
//	void createCommandBuffers()
//	{
//		commandBuffers.clear();
//		vk::CommandBufferAllocateInfo allocInfo{ .commandPool = commandPool, .level = vk::CommandBufferLevel::ePrimary, .commandBufferCount = MAX_FRAMES_IN_FLIGHT };
//		commandBuffers = vk::raii::CommandBuffers(device, allocInfo);
//	}
//
//	void recordCommandBuffer(uint32_t imageIndex)
//	{
//		auto& commandBuffer = commandBuffers[frameIndex];
//		commandBuffer.begin({});
//		// Before starting rendering, transition the swapchain image to COLOR_ATTACHMENT_OPTIMAL
//		transition_image_layout(
//			imageIndex,
//			vk::ImageLayout::eUndefined,
//			vk::ImageLayout::eColorAttachmentOptimal,
//			{},                                                        // srcAccessMask (no need to wait for previous operations)
//			vk::AccessFlagBits2::eColorAttachmentWrite,                // dstAccessMask
//			vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
//			vk::PipelineStageFlagBits2::eColorAttachmentOutput         // dstStage
//		);
//		vk::ClearValue              clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
//		vk::RenderingAttachmentInfo attachmentInfo = {
//			.imageView = swapChainImageViews[imageIndex],
//			.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
//			.loadOp = vk::AttachmentLoadOp::eClear,
//			.storeOp = vk::AttachmentStoreOp::eStore,
//			.clearValue = clearColor };
//		vk::RenderingInfo renderingInfo = {
//			.renderArea = {.offset = {0, 0}, .extent = swapChainExtent},
//			.layerCount = 1,
//			.colorAttachmentCount = 1,
//			.pColorAttachments = &attachmentInfo };
//		commandBuffer.beginRendering(renderingInfo);
//		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline);
//		////// faire les push constant apres bindpipeline
//		commandBuffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(swapChainExtent.width), static_cast<float>(swapChainExtent.height), 0.0f, 1.0f));
//		commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), swapChainExtent));
//
//		for (size_t i = 0; i < meshes.size(); ++i)
//		{
//			glm::mat4 model;
//
//			if (i == 0) {
//				model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
//			}
//			else {
//				model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//			}
//
//			commandBuffer.pushConstants<glm::mat4>(
//				*pipelineLayout,
//				vk::ShaderStageFlagBits::eVertex,
//				0,
//				model
//			);
//		}
//
//		for (const auto& mesh : meshes)
//		{
//			commandBuffer.bindVertexBuffers(0, *mesh.vertexBuffer, { 0 });
//			commandBuffer.bindIndexBuffer(*mesh.indexBuffer, 0, vk::IndexType::eUint32);
//			commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, *descriptorSets[frameIndex], nullptr);
//			commandBuffer.drawIndexed(mesh.indices.size(), 1, 0, 0, 0);
//		}
//		commandBuffer.endRendering();
//		// After rendering, transition the swapchain image to PRESENT_SRC
//		transition_image_layout(
//			imageIndex,
//			vk::ImageLayout::eColorAttachmentOptimal,
//			vk::ImageLayout::ePresentSrcKHR,
//			vk::AccessFlagBits2::eColorAttachmentWrite,                // srcAccessMask
//			{},                                                        // dstAccessMask
//			vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
//			vk::PipelineStageFlagBits2::eBottomOfPipe                  // dstStage
//		);
//		commandBuffer.end();
//	}
//
//	void transition_image_layout(
//		uint32_t                imageIndex,
//		vk::ImageLayout         old_layout,
//		vk::ImageLayout         new_layout,
//		vk::AccessFlags2        src_access_mask,
//		vk::AccessFlags2        dst_access_mask,
//		vk::PipelineStageFlags2 src_stage_mask,
//		vk::PipelineStageFlags2 dst_stage_mask)
//	{
//		vk::ImageMemoryBarrier2 barrier = {
//			.srcStageMask = src_stage_mask,
//			.srcAccessMask = src_access_mask,
//			.dstStageMask = dst_stage_mask,
//			.dstAccessMask = dst_access_mask,
//			.oldLayout = old_layout,
//			.newLayout = new_layout,
//			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
//			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
//			.image = swapChainImages[imageIndex],
//			.subresourceRange = {
//				   .aspectMask = vk::ImageAspectFlagBits::eColor,
//				   .baseMipLevel = 0,
//				   .levelCount = 1,
//				   .baseArrayLayer = 0,
//				   .layerCount = 1} };
//		vk::DependencyInfo dependency_info = {
//			.dependencyFlags = {},
//			.imageMemoryBarrierCount = 1,
//			.pImageMemoryBarriers = &barrier };
//		commandBuffers[frameIndex].pipelineBarrier2(dependency_info);
//	}
//
//	void createSyncObjects()
//	{
//		assert(presentCompleteSemaphores.empty() && renderFinishedSemaphores.empty() && inFlightFences.empty());
//
//		for (size_t i = 0; i < swapChainImages.size(); i++)
//		{
//			renderFinishedSemaphores.emplace_back(device, vk::SemaphoreCreateInfo());
//		}
//
//		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//		{
//			presentCompleteSemaphores.emplace_back(device, vk::SemaphoreCreateInfo());
//			inFlightFences.emplace_back(device, vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled });
//		}
//	}
//
//	void updateUniformBuffer(uint32_t currentImage)
//	{
//		static auto startTime = std::chrono::high_resolution_clock::now();
//
//		auto  currentTime = std::chrono::high_resolution_clock::now();
//		float time = std::chrono::duration<float>(currentTime - startTime).count();
//
//		UniformBufferObject ubo{};
//		ubo.model = rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//		ubo.view = camera->getViewMatrix();
//		ubo.proj = camera->getProjectionMatrix(
//			static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height)
//		);
//
//		ubo.proj[1][1] *= -1;
//
//		memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
//	}
//
//	void drawFrame()
//	{
//		// Note: inFlightFences, presentCompleteSemaphores, and commandBuffers are indexed by frameIndex,
//		//       while renderFinishedSemaphores is indexed by imageIndex
//		auto fenceResult = device.waitForFences(*inFlightFences[frameIndex], vk::True, UINT64_MAX);
//		if (fenceResult != vk::Result::eSuccess)
//		{
//			throw std::runtime_error("failed to wait for fence!");
//		}
//
//		auto [result, imageIndex] = swapChain.acquireNextImage(UINT64_MAX, *presentCompleteSemaphores[frameIndex], nullptr);
//
//		// Due to VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS being defined, eErrorOutOfDateKHR can be checked as a result
//		// here and does not need to be caught by an exception.
//		if (result == vk::Result::eErrorOutOfDateKHR)
//		{
//			recreateSwapChain();
//			return;
//		}
//		// On other success codes than eSuccess and eSuboptimalKHR we just throw an exception.
//		// On any error code, aquireNextImage already threw an exception.
//		if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
//		{
//			assert(result == vk::Result::eTimeout || result == vk::Result::eNotReady);
//			throw std::runtime_error("failed to acquire swap chain image!");
//		}
//		updateUniformBuffer(frameIndex);
//
//		// Only reset the fence if we are submitting work
//		device.resetFences(*inFlightFences[frameIndex]);
//
//		commandBuffers[frameIndex].reset();
//		recordCommandBuffer(imageIndex);
//
//		vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
//		const vk::SubmitInfo   submitInfo{ .waitSemaphoreCount = 1,
//										  .pWaitSemaphores = &*presentCompleteSemaphores[frameIndex],
//										  .pWaitDstStageMask = &waitDestinationStageMask,
//										  .commandBufferCount = 1,
//										  .pCommandBuffers = &*commandBuffers[frameIndex],
//										  .signalSemaphoreCount = 1,
//										  .pSignalSemaphores = &*renderFinishedSemaphores[imageIndex] };
//		queue.submit(submitInfo, *inFlightFences[frameIndex]);
//
//		const vk::PresentInfoKHR presentInfoKHR{ .waitSemaphoreCount = 1,
//												.pWaitSemaphores = &*renderFinishedSemaphores[imageIndex],
//												.swapchainCount = 1,
//												.pSwapchains = &*swapChain,
//												.pImageIndices = &imageIndex };
//		result = queue.presentKHR(presentInfoKHR);
//		// Due to VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS being defined, eErrorOutOfDateKHR can be checked as a result
//		// here and does not need to be caught by an exception.
//		if ((result == vk::Result::eSuboptimalKHR) || (result == vk::Result::eErrorOutOfDateKHR) || framebufferResized)
//		{
//			framebufferResized = false;
//			recreateSwapChain();
//		}
//		else
//		{
//			// There are no other success codes than eSuccess; on any error code, presentKHR already threw an exception.
//			assert(result == vk::Result::eSuccess);
//		}
//		frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
//	}
//
//	[[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const
//	{
//		vk::ShaderModuleCreateInfo createInfo{ .codeSize = code.size(), .pCode = reinterpret_cast<const uint32_t*>(code.data()) };
//		vk::raii::ShaderModule     shaderModule{ device, createInfo };
//
//		return shaderModule;
//	}
//
//	static uint32_t chooseSwapMinImageCount(vk::SurfaceCapabilitiesKHR const& surfaceCapabilities)
//	{
//		auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
//		if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount))
//		{
//			minImageCount = surfaceCapabilities.maxImageCount;
//		}
//		return minImageCount;
//	}
//
//	static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
//	{
//		assert(!availableFormats.empty());
//		const auto formatIt = std::ranges::find_if(
//			availableFormats,
//			[](const auto& format) { return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; });
//		return formatIt != availableFormats.end() ? *formatIt : availableFormats[0];
//	}
//
//	static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
//	{
//		assert(std::ranges::any_of(availablePresentModes, [](auto presentMode) { return presentMode == vk::PresentModeKHR::eFifo; }));
//		return std::ranges::any_of(availablePresentModes,
//			[](const vk::PresentModeKHR value) { return vk::PresentModeKHR::eMailbox == value; }) ?
//			vk::PresentModeKHR::eMailbox :
//			vk::PresentModeKHR::eFifo;
//	}
//
//	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
//	{
//		if (capabilities.currentExtent.width != 0xFFFFFFFF)
//		{
//			return capabilities.currentExtent;
//		}
//		int width, height;
//
//		window->GetFramebufferSize(width, height);
//
//		return {
//			std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
//			std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height) };
//	}
//
//	std::vector<const char*> getRequiredExtensions()
//	{
//		uint32_t glfwExtensionCount = 0;
//		auto     glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//		if (enableValidationLayers)
//		{
//			extensions.push_back(vk::EXTDebugUtilsExtensionName);
//		}
//
//		return extensions;
//	}
//
//	static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT type, const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData, void*)
//	{
//		if (severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError || severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
//		{
//			std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;
//		}
//
//		return vk::False;
//	}
//
//	static std::vector<char> readFile(const std::string& filename)
//	{
//		std::ifstream file(filename, std::ios::ate | std::ios::binary);
//		if (!file.is_open())
//		{
//			throw std::runtime_error("failed to open file! actualPath : " + std::filesystem::current_path().string() + " try directory : " + filename);
//		}
//		std::vector<char> buffer(file.tellg());
//		file.seekg(0, std::ios::beg);
//		file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
//		file.close();
//		return buffer;
//	}
//};


//void step0()
//{
//	try
//	{
//		HelloTriangleApplication app;
//		app.run();
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << e.what() << std::endl;
//	}
//
//}

#endif
