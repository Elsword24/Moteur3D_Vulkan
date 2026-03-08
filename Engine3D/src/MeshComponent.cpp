#include "MeshComponent.h"

#include "mesh.h"
#include "Event.h"
#include "EventBus.h"

MeshComponent::MeshComponent(const std::string& path)
{
	Mesh mesh;
	mesh.LoadModel(path);

	m_vertices.reserve(mesh.GetVertices().capacity());
	m_indices.reserve(mesh.GetIndices().capacity());
	m_vertices = std::move(mesh.GetVertices());
	m_indices = std::move(mesh.GetIndices());
}

void MeshComponent::OnDestroy()
{
	MeshDestroyEvent event(this);
	EventBus::Get().PublishEvent(event);
}

vk::raii::DeviceMemory& MeshComponent::GetIndexBufferMemory()
{
	return m_IndexBufferMemory;
}

vk::raii::Buffer& MeshComponent::GetIndexBuffer()
{
	return m_IndexBuffer;
}

vk::raii::DeviceMemory& MeshComponent::GetVertexBufferMemory()
{
	return m_VertexBufferMemory;
}

vk::raii::Buffer& MeshComponent::GetVertexBuffer()
{
	return m_VertexBuffer;
}

const std::vector<uint32_t>& MeshComponent::GetIndices() const
{
	return m_indices;
}

const std::vector<Vertex>& MeshComponent::GetVertices() const
{
	return m_vertices;
}

void MeshComponent::OnInitialize()
{
	MeshCreatedEvent event(this);
	EventBus::Get().PublishEvent(event);
}


