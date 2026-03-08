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




#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif



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


	void createInstance(const char* Title);

	void setupDebugMessenger();

	void createSurface(GLFWwindow* glfwWindow);

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSwapChain();

	void createImageViews();

	void createCommandPool();
};

#endif

