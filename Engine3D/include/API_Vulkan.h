#ifndef API_VULKAN_H
#define API_VULKAN_H
#pragma once

#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <chrono>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "BaseComponent.h"



#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif


struct MeshVulkan
{
	vk::raii::Buffer vertexBuffer = nullptr;
	vk::raii::DeviceMemory vertexBufferMemory = nullptr;
	vk::raii::Buffer indexBuffer = nullptr;
	vk::raii::DeviceMemory indicesBufferMemory = nullptr;
	uint32_t index = 0;
};


std::vector<MeshVulkan> meshVulkans;


class VulkanRAII
{

	
private:
	GLFWwindow						 *m_window = nullptr;
	vk::raii::Context                context;
	vk::raii::Instance               instance = nullptr;
	const std::vector<char const*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

private:
	vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

private:
	vk::raii::SurfaceKHR             surface = nullptr;

private:
	vk::raii::PhysicalDevice         physicalDevice = nullptr;
	std::vector<const char*> requiredDeviceExtension = { vk::KHRSwapchainExtensionName };

private:
	vk::raii::Device                 device = nullptr;
	uint32_t                         queueIndex = ~0;
	vk::raii::Queue                  queue = nullptr;

private:
	vk::raii::SwapchainKHR           swapChain = nullptr;
	std::vector<vk::Image>           swapChainImages;
	vk::SurfaceFormatKHR             swapChainSurfaceFormat;
	vk::Extent2D                     swapChainExtent;

private:
	std::vector<vk::raii::ImageView> swapChainImageViews;

private:
	vk::raii::CommandPool                commandPool = nullptr;

private:
	std::vector<const char*> getRequiredExtensions();

	static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback
	(
		vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
		vk::DebugUtilsMessageTypeFlagsEXT type,
		const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*
	);

	static uint32_t chooseSwapMinImageCount(vk::SurfaceCapabilitiesKHR const& surfaceCapabilities);

	static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

	static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

public:

	VulkanRAII(GLFWwindow* window);
	~VulkanRAII() = default;

	const vk::raii::Device& GetDevice() const;
	const vk::SurfaceFormatKHR& GetSwapChainSurfaceFormat() const;
	const vk::raii::PhysicalDevice& GetPhysicalDevice() const;
	const vk::raii::CommandPool& GetCommandPool() const;
	const std::vector<vk::raii::ImageView>& GetSwapChainImageViews() const;
	const vk::Extent2D& GetSwapChainExtent() const;
	const std::vector<vk::Image>& GetSwapChainImages() const;
	const vk::raii::SwapchainKHR& GetSwapChain() const;
	const vk::raii::Queue& GetQueue() const;
	//TODO : A Déplacer 
	/*void Vulkan(const std::vector<Vertex>& vertices, const std::vector<uint32_t> indices)
	{
		MeshVulkan meshVulkan;

		meshVulkan.index = static_cast<uint32_t>(indices.size());
		createVertexBuffer(vertices, meshVulkan.vertexBuffer, meshVulkan.vertexBufferMemory);
		createIndexBuffer(indices, meshVulkan.indexBuffer, meshVulkan.indicesBufferMemory);

		meshVulkans.push_back(std::move(meshVulkan));
	}*/
	

	/*void cleanupSwapChain()
	{
		swapChainImageViews.clear();
		swapChain = nullptr;
	}

	void recreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(m_window, &width, &height);
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(m_window, &width, &height);
			glfwWaitEvents();
		}
		device.waitIdle();

		cleanupSwapChain();
		createSwapChain();
		createImageViews();
	}*/

	void createInstance(const char* Title);

	void setupDebugMessenger();

	void createSurface(GLFWwindow* glfwWindow);

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSwapChain();

	void createImageViews();

	void createCommandPool();


	//Dans MeshComponent
	void createVertexBuffer(std::vector<Vertex> vertices, vk::raii::Buffer& vertexBuffer, vk::raii::DeviceMemory& vertexBufferMemory)
	{
		vk::DeviceSize         bufferSize = sizeof(vertices[0]) * vertices.size();
		vk::raii::Buffer       stagingBuffer({});
		vk::raii::DeviceMemory stagingBufferMemory({});

		createBuffer
		(
			bufferSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			stagingBuffer,
			stagingBufferMemory
		); 

		void* dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);
		memcpy(dataStaging, vertices.data(), bufferSize);
		stagingBufferMemory.unmapMemory();

		createBuffer
		(
			bufferSize,
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
			vk::MemoryPropertyFlagBits::eDeviceLocal,
			vertexBuffer, 
			vertexBufferMemory
		);

		copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	}

	//Dans MeshComponent
	void createIndexBuffer(std::vector<uint32_t> indices, vk::raii::Buffer& indexBuffer, vk::raii::DeviceMemory& indexBufferMemory)
	{
		vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();
		vk::raii::Buffer       stagingBuffer({});
		vk::raii::DeviceMemory stagingBufferMemory({});
		createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

		void* data = stagingBufferMemory.mapMemory(0, bufferSize);
		memcpy(data, indices.data(), (size_t)bufferSize);
		stagingBufferMemory.unmapMemory();

		createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer, indexBufferMemory);

		copyBuffer(stagingBuffer, indexBuffer, bufferSize);
	}

	

	//Dans MeshComponent
	void copyBuffer(vk::raii::Buffer& srcBuffer, vk::raii::Buffer& dstBuffer, vk::DeviceSize size)
	{
		vk::CommandBufferAllocateInfo allocInfo
		{ 
			.commandPool = commandPool,
			.level = vk::CommandBufferLevel::ePrimary,
			.commandBufferCount = 1 
		};
		vk::raii::CommandBuffer       commandCopyBuffer = std::move(device.allocateCommandBuffers(allocInfo).front());
		commandCopyBuffer.begin(vk::CommandBufferBeginInfo{ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
		commandCopyBuffer.copyBuffer(*srcBuffer, *dstBuffer, vk::BufferCopy(0, 0, size));
		commandCopyBuffer.end();
		queue.submit(vk::SubmitInfo{ .commandBufferCount = 1, .pCommandBuffers = &*commandCopyBuffer }, nullptr);
		queue.waitIdle();
	}

	
	

	

};

#endif

