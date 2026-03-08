#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H
#pragma once
#include<string>
#include <vector>

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#include "Component.h"
#include "EventListener.h"

class Mesh;
struct Vertex;

class MeshComponent : public Component
{
private:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

private:
	vk::raii::Buffer m_VertexBuffer = nullptr;
	vk::raii::DeviceMemory m_VertexBufferMemory = nullptr;
	vk::raii::Buffer m_IndexBuffer = nullptr;
	vk::raii::DeviceMemory m_IndexBufferMemory = nullptr;

public:
	MeshComponent(const std::string& path);
	
	void OnDestroy() override;

	const std::vector<Vertex>& GetVertices() const;

	const std::vector<uint32_t>& GetIndices() const;

	vk::raii::Buffer& GetVertexBuffer();

	vk::raii::DeviceMemory& GetVertexBufferMemory();

	vk::raii::Buffer& GetIndexBuffer();

	vk::raii::DeviceMemory& GetIndexBufferMemory();
};

#endif
