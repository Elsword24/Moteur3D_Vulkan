#include <cstdint>
#include <stdexcept>
#include <iostream>

#include "Engine.hpp"

//#include "Renderer.h"
//#include "BaseComponent.h"
//#include "window.h"



constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

//void launchVulkan( VukanRAII& app, Window* window, int width, int height )
//{
//	Mesh monkey;
//	monkey.LoadModel("./Assets/monkey.obj");
//
//	Mesh cube;
//	cube.LoadModel("./Assets/box.obj");
//
//	app.createInstance();
//	app.setupDebugMessenger();
//	app.createSurface(window->getGLFWWindow());
//	app.pickPhysicalDevice();
//	app.createLogicalDevice();
//	app.createSwapChain(width, height);
//	app.createImageViews();
//	app.createDescriptorSetLayout();
//	app.createGraphicsPipeline();
//	//app.createTextureImage();
//	app.createCommandPool();
//	app.Vulkan(monkey.GetVertices(), monkey.GetIndices());
//	app.Vulkan(cube.GetVertices(), cube.GetIndices());
//	app.createUniformBuffers();
//	app.createDescriptorPool();
//	app.createDescriptorSets();
//	app.createCommandBuffers();
//	app.createSyncObjects();
//}

//int main()
//{
//	try
//	{
//		Window window(WIDTH, HEIGHT);
//		VulkanRAII app;
//
//		Entity* camera = new Entity("mainCamera");
//		auto transform = camera->AddComponent<TransformComponent>();
//		camera->AddComponent<CameraComponent>();
//		camera->AddComponent<InputComponent>();
//		camera->AddComponent<MouseComponent>();
//
//		transform->SetPosition(glm::vec3(-1, 1, 10));
//		
//		//app.initWindow();
//
//		//window.initWindow();
//		glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//		auto& inputManager = InputMapper::GetInstance();
//		inputManager.Init(window.getGLFWWindow());
//
//		app.sceneObjects.push_back(std::make_pair(0, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -2.0f))));
//		app.sceneObjects.push_back(std::make_pair(1, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -2.0f))));
//
//		//TODO : a enlever
//		int width = 1920, height = 1080;
//
//		launchVulkan(app, &window, width, height);
//		app.camTest = camera;
//		while (!window.WindowClosed())
//		{
//			window.PollEvent();
//
//			inputManager.Update();
//			//camera->processInput(*window, *camera, 0.16f);
//
//			// Keyboard input is handled in the InputComponent, so we update it here
//			//auto Cam = app.camTest->GetComponent<InputComponent>();
//			//Cam->Update(0.16f);
//
//			//Mouse input is handled in the MouseComponent, so we update it here
//			auto MouseInputs = app.camTest->GetComponent<MouseComponent>();
//			MouseInputs->Update(0.16f);
//
//			int width = 0, height = 0;
//			window.GetFramebufferSize(width, height);
//			while (width == 0 || height == 0)
//			{
//				window.GetFramebufferSize(width, height);
//				window.WaitEvents();
//			}
//
//			app.drawFrame(width, height);
//
//		}
//
//		app.device.waitIdle();
//		window.cleanup();
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << e.what() << "\n";
//	}
//}

int main()
{
	try
	{
		EngineQVY Game("RailShooter", WIDTH, HEIGHT);
		Game.RunGameLoop();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}