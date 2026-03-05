#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include "Vulkan.h"
#include "mesh.h"

class Renderer
{
private:
	VulkanRAII* m_ObserverVulkan = nullptr;

private:
public:
	Renderer(VulkanRAII* m_ObserverVulkan)
	{
		
	}
	~Renderer() = default;

};

#endif
