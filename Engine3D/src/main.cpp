#include "Renderer.h"
#include "BaseComponent.h"
#include "window.h"
#include "Physics.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


void launchVulkan( HelloTriangleApplication& app, Window* window, int width, int height )
{
	Mesh monkey;
	monkey.LoadModel("./Assets/monkey.obj");

	Mesh cube;
	cube.LoadModel("./Assets/box.obj");

	app.createInstance();
	app.setupDebugMessenger();
	app.createSurface(window->getGLFWWindow());
	app.pickPhysicalDevice();
	app.createLogicalDevice();
	app.createSwapChain(width, height);
	app.createImageViews();
	app.createDescriptorSetLayout();
	app.createGraphicsPipeline();
	//app.createTextureImage();
	app.createCommandPool();
	app.Vulkan(monkey.GetVertices(), monkey.GetIndices());
	app.Vulkan(cube.GetVertices(), cube.GetIndices());
	app.createUniformBuffers();
	app.createDescriptorPool();
	app.createDescriptorSets();
	app.createCommandBuffers();
	app.createSyncObjects();
}

int main()
{
	try
	{
		Window window;
		HelloTriangleApplication app;
		Physics::PhysicsSystem m_PhysicsSystem;

		Entity* camera = new Entity("mainCamera");
		auto transform = camera->AddComponent<TransformComponent>();
		camera->AddComponent<CameraComponent>();
		camera->AddComponent<InputComponent>();
		camera->AddComponent<MouseComponent>();

		transform->SetPosition(glm::vec3(-1, 1, 10));

		window.initWindow();
		glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		auto& inputManager = InputMapper::GetInstance();
		inputManager.Init(window.getGLFWWindow());
		MouseComponent::s_Window = &window;
		MouseComponent::s_Physics = &m_PhysicsSystem;

		app.sceneObjects.push_back(std::make_pair(0, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -2.0f))));
		app.sceneObjects.push_back(std::make_pair(1, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -2.0f))));
		{
			auto rb1 = m_PhysicsSystem.CreateRigidBody();
			rb1->SetKinematic(true);
			rb1->SetGravityEnabled(false);
			rb1->SetPosition(glm::vec3(-2.0f, 0.0f, -2.0f));
			rb1->SetCollider(std::make_shared<Physics::BoxCollider>(glm::vec3(1.0f)));

			auto rb2 = m_PhysicsSystem.CreateRigidBody();
			rb2->SetKinematic(true);
			rb2->SetGravityEnabled(false);
			rb2->SetPosition(glm::vec3(2.0f, 0.0f, -2.0f));
			rb2->SetCollider(std::make_shared<Physics::BoxCollider>(glm::vec3(1.0f)));
		}

		int width = 1920, height = 1080;

		launchVulkan(app, &window, width, height);
		app.camTest = camera;

		Physics::PhysicsSystem& GetPhysicsSystem = m_PhysicsSystem;

		//m_PhysicsSystem.Initialize();

		while (!window.WindowClosed())
		{
			window.PollEvent();

			inputManager.Update();

			// Keyboard input is handled in the InputComponent, so we update it here
			//auto Cam = app.camTest->GetComponent<InputComponent>();
			//Cam->Update(0.16f);

			//Mouse input is handled in the MouseComponent, so we update it here
			auto MouseInputs = app.camTest->GetComponent<MouseComponent>();
			MouseInputs->Update(0.16f);

			int width = 0, height = 0;
			window.GetFramebufferSize(width, height);
			while (width == 0 || height == 0)
			{
				window.GetFramebufferSize(width, height);
				window.WaitEvents();
			}

			app.drawFrame(width, height);

		}

		app.device.waitIdle();
		window.cleanup();
		//m_PhysicsSystem.Shutdown();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
}