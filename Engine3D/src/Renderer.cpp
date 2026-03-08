#include "Renderer.h"

#include <print>

#include "API_Vulkan.h"
#include "mesh.h"
#include "vertex.h"
#include "Entity.h"
#include "BaseComponent.h"
#include "EventBus.h"
#include "MeshComponent.h"


vk::VertexInputBindingDescription Renderer::getBindingDescription()
{
	return vk::VertexInputBindingDescription
	{
		.binding = 0,
		.stride = sizeof(Vertex),
		.inputRate = vk::VertexInputRate::eVertex
	};
}

std::array<vk::VertexInputAttributeDescription, 3> Renderer::getAttributeDescriptions()
{
	return
	{
		vk::VertexInputAttributeDescription(0,0,vk::Format::eR32G32B32Sfloat,offsetof(Vertex, pos)),
		vk::VertexInputAttributeDescription(1,0,vk::Format::eR32G32B32Sfloat,offsetof(Vertex, normal)),
		vk::VertexInputAttributeDescription(2,0,vk::Format::eR32G32Sfloat,offsetof(Vertex, uv))
	};
}

void Renderer::createDescriptorSetLayout()
{
	vk::DescriptorSetLayoutBinding    uboLayoutBinding
	(
		0,
		vk::DescriptorType::eUniformBuffer,
		1,
		vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
		nullptr
	);

	vk::DescriptorSetLayoutCreateInfo layoutInfo
	{ 
		.bindingCount = 1, 
		.pBindings = &uboLayoutBinding 
	};

	descriptorSetLayout = vk::raii::DescriptorSetLayout(m_ObserverVulkan->GetDevice(), layoutInfo);
}

void Renderer::createGraphicsPipeline()
{
	vk::raii::ShaderModule shaderModule = createShaderModule(readFile("shaders/slang.spv"));

	vk::PipelineShaderStageCreateInfo vertShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eVertex, .module = shaderModule, .pName = "vertMain" };
	vk::PipelineShaderStageCreateInfo fragShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eFragment, .module = shaderModule, .pName = "fragMain" };
	vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	auto                                     bindingDescription = getBindingDescription();
	auto                                     attributeDescriptions = getAttributeDescriptions();
	vk::PipelineVertexInputStateCreateInfo   vertexInputInfo{ .vertexBindingDescriptionCount = 1, .pVertexBindingDescriptions = &bindingDescription, .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()), .pVertexAttributeDescriptions = attributeDescriptions.data() };
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly{ .topology = vk::PrimitiveTopology::eTriangleList };
	vk::PipelineViewportStateCreateInfo      viewportState{ .viewportCount = 1, .scissorCount = 1 };

	vk::PipelineRasterizationStateCreateInfo rasterizer{ .depthClampEnable = vk::False, .rasterizerDiscardEnable = vk::False, .polygonMode = vk::PolygonMode::eFill, .cullMode = vk::CullModeFlagBits::eNone, .frontFace = vk::FrontFace::eCounterClockwise, .depthBiasEnable = vk::False, .depthBiasSlopeFactor = 1.0f, .lineWidth = 1.0f };

	vk::PipelineMultisampleStateCreateInfo multisampling{ .rasterizationSamples = vk::SampleCountFlagBits::e1, .sampleShadingEnable = vk::False };

	vk::PipelineColorBlendAttachmentState colorBlendAttachment{ .blendEnable = vk::False,
		.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };

	vk::PipelineColorBlendStateCreateInfo colorBlending{ .logicOpEnable = vk::False, .logicOp = vk::LogicOp::eCopy, .attachmentCount = 1, .pAttachments = &colorBlendAttachment };

	std::vector dynamicStates = {
		vk::DynamicState::eViewport,
		vk::DynamicState::eScissor };
	vk::PipelineDynamicStateCreateInfo dynamicState{ .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()), .pDynamicStates = dynamicStates.data() };

	vk::PushConstantRange pushConstant{ .stageFlags = vk::ShaderStageFlagBits::eVertex, .offset = 0, .size = sizeof(glm::mat4) };

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo{ .setLayoutCount = 1, .pSetLayouts = &*descriptorSetLayout, .pushConstantRangeCount = 1, .pPushConstantRanges = &pushConstant };

	pipelineLayout = vk::raii::PipelineLayout(m_ObserverVulkan->GetDevice(), pipelineLayoutInfo);

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
			.renderPass = nullptr},
		{
			.colorAttachmentCount = 1, 
			.pColorAttachmentFormats = &m_ObserverVulkan->GetSwapChainSurfaceFormat().format
		} 
	};

	graphicsPipeline = vk::raii::Pipeline(m_ObserverVulkan->GetDevice(), nullptr, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
}

void Renderer::createUniformBuffers()
{
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	uniformBuffersMapped.clear();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DeviceSize         bufferSize = sizeof(UniformBufferObject);
		vk::raii::Buffer       buffer({});
		vk::raii::DeviceMemory bufferMem({});
		createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, buffer, bufferMem);
		uniformBuffers.emplace_back(std::move(buffer));
		uniformBuffersMemory.emplace_back(std::move(bufferMem));
		uniformBuffersMapped.emplace_back(uniformBuffersMemory[i].mapMemory(0, bufferSize));
	}
}

void Renderer::createDescriptorPool()
{
	vk::DescriptorPoolSize       poolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT);
	vk::DescriptorPoolCreateInfo poolInfo{ .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, .maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT), .poolSizeCount = 1, .pPoolSizes = &poolSize };
	descriptorPool = vk::raii::DescriptorPool(m_ObserverVulkan->GetDevice(), poolInfo);
}

void Renderer::createDescriptorSets()
{
	std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout);
	vk::DescriptorSetAllocateInfo        allocInfo{ .descriptorPool = descriptorPool, .descriptorSetCount = static_cast<uint32_t>(layouts.size()), .pSetLayouts = layouts.data() };

	descriptorSets = m_ObserverVulkan->GetDevice().allocateDescriptorSets(allocInfo);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DescriptorBufferInfo bufferInfo{ .buffer = uniformBuffers[i], .offset = 0, .range = sizeof(UniformBufferObject) };
		vk::WriteDescriptorSet   descriptorWrite{ .dstSet = descriptorSets[i], .dstBinding = 0, .dstArrayElement = 0, .descriptorCount = 1, .descriptorType = vk::DescriptorType::eUniformBuffer, .pBufferInfo = &bufferInfo };
		m_ObserverVulkan->GetDevice().updateDescriptorSets(descriptorWrite, {});
	}
}

void Renderer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
	vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory)
{
	vk::BufferCreateInfo bufferInfo{ .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive };
	buffer = vk::raii::Buffer(m_ObserverVulkan->GetDevice(), bufferInfo);
	vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();
	vk::MemoryAllocateInfo allocInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties) };
	bufferMemory = vk::raii::DeviceMemory(m_ObserverVulkan->GetDevice(), allocInfo);
	buffer.bindMemory(bufferMemory, 0);
}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
	vk::PhysicalDeviceMemoryProperties memProperties = m_ObserverVulkan->GetPhysicalDevice().getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void Renderer::createCommandBuffers()
{
	commandBuffers.clear();
	vk::CommandBufferAllocateInfo allocInfo{ .commandPool = m_ObserverVulkan->GetCommandPool(), .level = vk::CommandBufferLevel::ePrimary, .commandBufferCount = MAX_FRAMES_IN_FLIGHT };
	commandBuffers = vk::raii::CommandBuffers(m_ObserverVulkan->GetDevice(), allocInfo);
}

void Renderer::recordCommandBuffer(uint32_t imageIndex)
{
	auto& commandBuffer = commandBuffers[frameIndex];
	commandBuffer.begin({});

	// Before starting rendering, transition the swapchain image to COLOR_ATTACHMENT_OPTIMAL
	transition_image_layout
	(
		imageIndex,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eColorAttachmentOptimal,
		{},                                                        // srcAccessMask (no need to wait for previous operations)
		vk::AccessFlagBits2::eColorAttachmentWrite,                // dstAccessMask
		vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
		vk::PipelineStageFlagBits2::eColorAttachmentOutput         // dstStage
	);

	vk::ClearValue              clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
	vk::RenderingAttachmentInfo attachmentInfo = 
	{
		.imageView = m_ObserverVulkan->GetSwapChainImageViews()[imageIndex],
		.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.clearValue = clearColor 
	};

	vk::RenderingInfo renderingInfo = 
	{
		.renderArea = {.offset = {0, 0}, .extent = m_ObserverVulkan->GetSwapChainExtent()},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &attachmentInfo 
	};

	commandBuffer.beginRendering(renderingInfo);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline);
	// faire les push constant apres bindpipeline
	commandBuffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(m_ObserverVulkan->GetSwapChainExtent().width), static_cast<float>(m_ObserverVulkan->GetSwapChainExtent().height), 0.0f, 1.0f));
	commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_ObserverVulkan->GetSwapChainExtent()));

	if (m_RenderComponent.empty())
		std::println("List of Render Component is Empty !!!");


	for (auto& Mesh : m_RenderComponent)
	{
		auto* transformComponent = Mesh->GetOwner()->GetComponent<TransformComponent>();
		assert(transformComponent && "MeshComponent requires a TransformComponent !!!!!");
		glm::mat4 transform = transformComponent->GetTransformMatrix();
		commandBuffer.pushConstants<glm::mat4>
			(
				*pipelineLayout,
				vk::ShaderStageFlagBits::eVertex,
				0,
				transform
			);

		commandBuffer.bindVertexBuffers(0, *Mesh->GetVertexBuffer(), { 0 });
		commandBuffer.bindIndexBuffer(*Mesh->GetIndexBuffer(), 0, vk::IndexType::eUint32);
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pipelineLayout, 0, *descriptorSets[frameIndex], nullptr);
		commandBuffer.drawIndexed(Mesh->GetIndices().size(), 1, 0, 0, 0);

	}



	commandBuffer.endRendering();

	// After rendering, transition the swapchain image to PRESENT_SRC
	transition_image_layout(
		imageIndex,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::ImageLayout::ePresentSrcKHR,
		vk::AccessFlagBits2::eColorAttachmentWrite,                // srcAccessMask
		{},                                                        // dstAccessMask
		vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
		vk::PipelineStageFlagBits2::eBottomOfPipe                  // dstStage
	);

	commandBuffer.end();
}

void Renderer::transition_image_layout(uint32_t imageIndex, vk::ImageLayout old_layout, vk::ImageLayout new_layout,
	vk::AccessFlags2 src_access_mask, vk::AccessFlags2 dst_access_mask, vk::PipelineStageFlags2 src_stage_mask,
	vk::PipelineStageFlags2 dst_stage_mask)
{
	vk::ImageMemoryBarrier2 barrier = {
		.srcStageMask = src_stage_mask,
		.srcAccessMask = src_access_mask,
		.dstStageMask = dst_stage_mask,
		.dstAccessMask = dst_access_mask,
		.oldLayout = old_layout,
		.newLayout = new_layout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = m_ObserverVulkan->GetSwapChainImages()[imageIndex],
		.subresourceRange = {
			.aspectMask = vk::ImageAspectFlagBits::eColor,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1} };
	vk::DependencyInfo dependency_info = {
		.dependencyFlags = {},
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &barrier };
	commandBuffers[frameIndex].pipelineBarrier2(dependency_info);
}

void Renderer::createSyncObjects()
{
	assert(presentCompleteSemaphores.empty() && renderFinishedSemaphores.empty() && inFlightFences.empty());

	for (size_t i = 0; i < m_ObserverVulkan->GetSwapChainImages().size(); i++)
	{
		renderFinishedSemaphores.emplace_back(m_ObserverVulkan->GetDevice(), vk::SemaphoreCreateInfo());
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		presentCompleteSemaphores.emplace_back(m_ObserverVulkan->GetDevice(), vk::SemaphoreCreateInfo());
		inFlightFences.emplace_back(m_ObserverVulkan->GetDevice(), vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled });
	}
}

void Renderer::updateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto  currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float>(currentTime - startTime).count();

	
	UniformBufferObject ubo{};

	float aspect = static_cast<float>(m_ObserverVulkan->GetSwapChainExtent().width) / static_cast<float>(m_ObserverVulkan->GetSwapChainExtent().height);
	if (m_activeCamera)
	{
		m_activeCamera->SetPerspective(45.0f, aspect, 0.1f, 1000.0f);

		ubo.view = m_activeCamera->GetViewMatrix();
		ubo.proj = m_activeCamera->GetProjectionMatrix();
		ubo.proj[1][1] *= -1;
	}

	//TODO : A METRRE DANS UN LIGHTCOMPONENT 
	

	ubo.light.posWorld = { 0.0f,2.0f,0.0f };
	ubo.light.color = { 1.0f,1.0f,1.0f };
	ubo.light.intensity = 1.0f;

	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

vk::raii::ShaderModule Renderer::createShaderModule(const std::vector<char>& code) const
{
	vk::ShaderModuleCreateInfo createInfo{ .codeSize = code.size(), .pCode = reinterpret_cast<const uint32_t*>(code.data()) };
	vk::raii::ShaderModule     shaderModule{ m_ObserverVulkan->GetDevice(), createInfo };

	return shaderModule;
}

std::vector<char> Renderer::readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file! actualPath : " + std::filesystem::current_path().string() + " try directory : " + filename);
	}
	std::vector<char> buffer(file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
	file.close();
	return buffer;
}


void Renderer::drawFrame()
{
	// Note: inFlightFences, presentCompleteSemaphores, and commandBuffers are indexed by frameIndex,
	//       while renderFinishedSemaphores is indexed by imageIndex
	auto fenceResult = m_ObserverVulkan->GetDevice().waitForFences(*inFlightFences[frameIndex], vk::True, UINT64_MAX);
	if (fenceResult != vk::Result::eSuccess)
	{
		throw std::runtime_error("failed to wait for fence!");
	}

	auto [result, imageIndex] = m_ObserverVulkan->GetSwapChain().acquireNextImage(UINT64_MAX, *presentCompleteSemaphores[frameIndex], nullptr);

	
	updateUniformBuffer(frameIndex);

	// Only reset the fence if we are submitting work
	m_ObserverVulkan->GetDevice().resetFences(*inFlightFences[frameIndex]);

	commandBuffers[frameIndex].reset();
	recordCommandBuffer(imageIndex);

	vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
	const vk::SubmitInfo   submitInfo{ .waitSemaphoreCount = 1,
		.pWaitSemaphores = &*presentCompleteSemaphores[frameIndex],
		.pWaitDstStageMask = &waitDestinationStageMask,
		.commandBufferCount = 1,
		.pCommandBuffers = &*commandBuffers[frameIndex],
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &*renderFinishedSemaphores[imageIndex] };
	m_ObserverVulkan->GetQueue().submit(submitInfo, *inFlightFences[frameIndex]);

	const vk::PresentInfoKHR presentInfoKHR{ .waitSemaphoreCount = 1,
		.pWaitSemaphores = &*renderFinishedSemaphores[imageIndex],
		.swapchainCount = 1,
		.pSwapchains = &*m_ObserverVulkan->GetSwapChain(),
		.pImageIndices = &imageIndex };
	result = m_ObserverVulkan->GetQueue().presentKHR(presentInfoKHR);

	frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::createVertexBuffer(std::vector<Vertex> vertices, vk::raii::Buffer& vertexBuffer,
	vk::raii::DeviceMemory& vertexBufferMemory)
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

void Renderer::createIndexBuffer(std::vector<uint32_t> indices, vk::raii::Buffer& indexBuffer,
	vk::raii::DeviceMemory& indexBufferMemory)
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

void Renderer::copyBuffer(vk::raii::Buffer& srcBuffer, vk::raii::Buffer& dstBuffer, vk::DeviceSize size)
{
	vk::CommandBufferAllocateInfo allocInfo
	{
		.commandPool = m_ObserverVulkan->GetCommandPool(),
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1
	};
	vk::raii::CommandBuffer       commandCopyBuffer = std::move(m_ObserverVulkan->GetDevice().allocateCommandBuffers(allocInfo).front());
	commandCopyBuffer.begin(vk::CommandBufferBeginInfo{ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
	commandCopyBuffer.copyBuffer(*srcBuffer, *dstBuffer, vk::BufferCopy(0, 0, size));
	commandCopyBuffer.end();
	m_ObserverVulkan->GetQueue().submit(vk::SubmitInfo{ .commandBufferCount = 1, .pCommandBuffers = &*commandCopyBuffer }, nullptr);
	m_ObserverVulkan->GetQueue().waitIdle();
}

void Renderer::HandleMeshCreated(MeshComponent* Mesh)
{
	createVertexBuffer(Mesh->GetVertices(), Mesh->GetVertexBuffer(), Mesh->GetVertexBufferMemory());
	createIndexBuffer(Mesh->GetIndices(), Mesh->GetIndexBuffer(), Mesh->GetIndexBufferMemory());

	assert(*Mesh->GetVertexBuffer() && "VertexBuffer creation failed!");
	assert(*Mesh->GetIndexBuffer() && "IndexBuffer creation failed!");

	m_RenderComponent.push_back(Mesh);
}

void Renderer::HandleMeshDestroy(MeshComponent* Mesh)
{
	m_RenderComponent.erase
	(
		std::remove_if
		(
			m_RenderComponent.begin(), m_RenderComponent.end(),[&](const MeshComponent* mesh)
			{
				return mesh == Mesh;
			}
		),
		m_RenderComponent.end()
	);
}

void Renderer::HandleCameraSet(CameraComponent* Camera)
{
	m_activeCamera = Camera;
}

void Renderer::HandleCameraRemove(CameraComponent* Camera)
{
	m_activeCamera = nullptr;
}


Renderer::Renderer(VulkanRAII* ObserverVulkan)
	:m_ObserverVulkan(ObserverVulkan)
{
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSyncObjects();

	EventBus::Get().AddListener(this, EventCategory::Application, 0);
}

void Renderer::OnEvent(const Event& event)
{
	EventDispatcher dispatcher(event);

	dispatcher.Dispatch<MeshCreatedEvent>([this](const MeshCreatedEvent& e)
		{
			HandleMeshCreated(e.GetTarget());
		}
	);

	dispatcher.Dispatch<MeshDestroyEvent>([this](const MeshDestroyEvent& e)
		{
			HandleMeshDestroy(e.GetTarget());
		}
	);

	dispatcher.Dispatch<CameraSetEvent>([this](const CameraSetEvent& e)
		{
			HandleCameraSet(e.GetTarget());
		}
	);

	dispatcher.Dispatch<CameraRemoveEvent>([this](const CameraRemoveEvent& e)
		{
			HandleCameraRemove(e.GetTarget());
		}
	);
}