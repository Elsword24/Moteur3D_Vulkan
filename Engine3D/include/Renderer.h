#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include <vector>
#include <iostream>
#include <utility>

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//namespace vk
//{
//	namespace raii
//	{
//		class DescriptorSetLayout;
//		class PipelineLayout;
//		class Pipeline;
//		class Buffer;
//		class DeviceMemory;
//		class DescriptorPool;
//		class DescriptorSet;
//		class CommandBuffer;
//		class Semaphore;
//		class Fence;
//	}
//}

//vk::raii::Taa;

class VulkanRAII;
class Entity;

class Renderer
{
private:
	VulkanRAII* m_ObserverVulkan = nullptr;

private:
	vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
	vk::raii::PipelineLayout      pipelineLayout = nullptr;
	vk::raii::Pipeline            graphicsPipeline = nullptr;

private:
	std::vector<vk::raii::Buffer>       uniformBuffers;
	std::vector<vk::raii::DeviceMemory> uniformBuffersMemory;
	std::vector<void*>                 uniformBuffersMapped;

private:
	vk::raii::DescriptorPool             descriptorPool = nullptr;
	std::vector<vk::raii::DescriptorSet> descriptorSets;

	

private:
	std::vector<vk::raii::CommandBuffer> commandBuffers;

private:
	std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
	std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
	std::vector<vk::raii::Fence>     inFlightFences;
	uint32_t                         frameIndex = 0;
	static constexpr int      MAX_FRAMES_IN_FLIGHT = 2;

private:
	//Dans Renderer
	bool framebufferResized = false;
private:
	//std::unique_ptr<Camera> camera;
	Entity* camTest = nullptr;
	std::vector<std::pair<uint32_t, glm::mat4>> sceneObjects;

private:
	vk::VertexInputBindingDescription getBindingDescription();

	std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions();

	void createDescriptorSetLayout();

	//Dans Renderer
	void createGraphicsPipeline();

	//Function to implement to add textures
	/*void createTextureImage()
	{

	}*/

	//Dans Renderer
	void createUniformBuffers();

	//Dans Renderer
	void createDescriptorPool();

	//Dans Renderer
	void createDescriptorSets();

	//Dans Renderer
	void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory);

	//Dans renderer
	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

	//TODO :
	//Dans Renderer MAIS A MODIFIER
	void createCommandBuffers();

	//TODO :
	//Dans Renderer MAIS A MODIFIER 
	void recordCommandBuffer(uint32_t imageIndex);

	//Dans Renderer
	void transition_image_layout(
		uint32_t                imageIndex,
		vk::ImageLayout         old_layout,
		vk::ImageLayout         new_layout,
		vk::AccessFlags2        src_access_mask,
		vk::AccessFlags2        dst_access_mask,
		vk::PipelineStageFlags2 src_stage_mask,
		vk::PipelineStageFlags2 dst_stage_mask);
	//TODO :
	//Dans Renderer MAIS A MODIFIER 
	void createSyncObjects();
	//TODO : 
	//Dans Renderer  A MODIFIER
	void updateUniformBuffer(uint32_t currentImage);

	//Dans Renderer
	[[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const;

	//Dans Renderer
	static std::vector<char> readFile(const std::string& filename);

	//TODO :
	// Dans la GameLoop
	void drawFrame();

public:
	Renderer(VulkanRAII* ObserverVulkan);
	~Renderer() = default;

};

#endif
