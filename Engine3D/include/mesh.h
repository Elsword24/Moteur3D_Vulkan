#pragma once
#include <rapidobj.hpp>
#include "vertex.h"


//struct MeshData
//{
//	std::vector<Vertex>   m_vertices;
//	std::vector<uint32_t> m_indices;
//};

class Mesh
{
public:

	Mesh LoadModel(const std::string& filePath)
	{
		Mesh mesh;
		m_vertices.clear();
		m_indices.clear();

		auto rapidObj = rapidobj::ParseFile(filePath);

		if (!std::filesystem::exists(filePath))
		{
			throw std::runtime_error("model file not found! " + std::filesystem::absolute(filePath).string());
		}

		if (rapidObj.error)
		{
			throw std::runtime_error("Failed to load model! " + rapidObj.error.line);
		}

		rapidobj::Triangulate(rapidObj);

		for (const auto& shape : rapidObj.shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};
				vertex.pos =
				{
					rapidObj.attributes.positions[3 * index.position_index + 0],
					rapidObj.attributes.positions[3 * index.position_index + 1],
					rapidObj.attributes.positions[3 * index.position_index + 2]

				};

				if (index.normal_index != -1)
				{
					vertex.normal =
					{
						rapidObj.attributes.normals[3 * index.normal_index + 0],
						rapidObj.attributes.normals[3 * index.normal_index + 1],
						rapidObj.attributes.normals[3 * index.normal_index + 2],
					};
				}
				else
				{
					vertex.normal = { 0.0f, 1.0f, 0.0f };
				};

				if (index.texcoord_index != -1)
				{
					vertex.uv =
					{
						rapidObj.attributes.texcoords[2 * index.texcoord_index + 0],
						1.0f - rapidObj.attributes.texcoords[2 * index.texcoord_index + 1]

					};
				}
				else
				{
					vertex.uv = { 0.0f, 0.0f };
				};

				m_indices.push_back(static_cast<uint32_t>(m_indices.size()));
				m_vertices.push_back(vertex);
			}
		}

		std::cout << "Loaded model with " << m_vertices.size() << " vertices." << std::endl;
		std::cout << "Loaded model with " << m_indices.size() << " indices." << std::endl;
		return mesh;
	}


	const std::vector<Vertex>& GetVertices() { return m_vertices; }
	const std::vector<uint32_t>& GetIndices() { return m_indices; }

private:
	// std::span plus leger (zero-copy, non-owning)
	std::vector<Vertex>   m_vertices;
	std::vector<uint32_t> m_indices;
};

