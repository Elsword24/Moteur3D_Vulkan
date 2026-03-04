#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Light
{
	alignas(16) glm::vec3 posWorld;
	alignas(16) glm::vec3 color;
	float intensity;
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct UniformBufferObject
{
	//glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	Light light;
};
