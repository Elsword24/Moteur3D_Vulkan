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

#include "EventListener.h"


class VulkanRAII;
class Entity;
class MeshComponent;
class CameraComponent;
struct Vertex;

class Renderer : public EventListener
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
	bool framebufferResized = false;

private:
	std::vector<MeshComponent*> m_RenderComponent;
	CameraComponent* m_activeCamera = nullptr;

private:
	vk::VertexInputBindingDescription getBindingDescription();

	std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions();

	void createDescriptorSetLayout();

	void createGraphicsPipeline();

	
	void createUniformBuffers();

	void createDescriptorPool();

	void createDescriptorSets();

	void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory);

	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

	void createCommandBuffers();

	void recordCommandBuffer(uint32_t imageIndex);

	void transition_image_layout(
		uint32_t                imageIndex,
		vk::ImageLayout         old_layout,
		vk::ImageLayout         new_layout,
		vk::AccessFlags2        src_access_mask,
		vk::AccessFlags2        dst_access_mask,
		vk::PipelineStageFlags2 src_stage_mask,
		vk::PipelineStageFlags2 dst_stage_mask);

	void createSyncObjects();
	
	void updateUniformBuffer(uint32_t currentImage);

	
	[[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const;

	
	static std::vector<char> readFile(const std::string& filename);


	void createVertexBuffer(std::vector<Vertex> vertices, vk::raii::Buffer& vertexBuffer, vk::raii::DeviceMemory& vertexBufferMemory);

	void createIndexBuffer(std::vector<uint32_t> indices, vk::raii::Buffer& indexBuffer, vk::raii::DeviceMemory& indexBufferMemory);


	void copyBuffer(vk::raii::Buffer& srcBuffer, vk::raii::Buffer& dstBuffer, vk::DeviceSize size);

private:
	void HandleMeshCreated(MeshComponent* Mesh);

	void HandleMeshDestroy(MeshComponent* Mesh);

	void HandleCameraSet(CameraComponent* Camera);

	void HandleCameraRemove(CameraComponent* Camera);

public:
	Renderer(VulkanRAII* ObserverVulkan);
	~Renderer() = default;
	void drawFrame();
	void OnEvent(const Event& event) override;
};

#endif
