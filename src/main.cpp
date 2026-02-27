#include "Renderer.h"

int main()
{
	try
	{
		HelloTriangleApplication app;

		Mesh monkey;
		monkey.LoadModel("../shaders/monkey.obj");

		Mesh cube;
		monkey.LoadModel("../shaders/box.obj");

		app.initWindow();
		app.createInstance();
		app.setupDebugMessenger();
		app.createSurface();
		app.pickPhysicalDevice();
		app.createLogicalDevice();
		app.createSwapChain();
		app.createImageViews();
		app.createDescriptorSetLayout();
		app.createGraphicsPipeline();
		app.createTextureImage();
		app.createCommandPool();
		app.Vulkan(monkey.GetVertices(), monkey.GetIndices());
		app.createUniformBuffers();
		app.createDescriptorPool();
		app.createDescriptorSets();
		app.createCommandBuffers();
		app.createSyncObjects();
		app.camera->setupInputCallbacks();
		app.mainLoop();
		app.cleanup();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}