#include "Renderer.h"

int main()
{
	try
	{
		HelloTriangleApplication app;

		Mesh monkey;
		monkey.LoadModel("../shaders/monkey.obj");

		Mesh cube;
		cube.LoadModel("../shaders/box.obj");
		
		app.initWindow();
		app.sceneObjects.push_back(std::make_pair(0, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f))));
		app.sceneObjects.push_back(std::make_pair(1, glm::translate(glm::mat4(0.5f), glm::vec3(2.0f, 0.0f, 0.0f))));

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
		app.Vulkan(cube.GetVertices(), cube.GetIndices());
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