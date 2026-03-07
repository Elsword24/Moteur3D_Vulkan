#include "Renderer.h"
#include "BaseComponent.h"
#include "window.h"
#include "Physics.h"
#include "SceneManager.h"
#include "EntityEventSystem.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "InputMapper.h"
#include "spline.h"

//constexpr uint32_t WIDTH = 800;
//constexpr uint32_t HEIGHT = 600;

void launchVulkan( HelloTriangleApplication& app, Window& window, int& width, int& height )
{
	//Mesh monkey;
	//monkey.LoadModel("Assets/monkey.obj");

	Mesh cube;
	cube.LoadModel("Assets/box.obj");
	Mesh cube1;
	cube1.LoadModel("Assets/box.obj");
	Mesh cube2;
	cube2.LoadModel("Assets/box.obj");
	Mesh cube3;
	cube3.LoadModel("Assets/box.obj");

	app.createInstance();
	app.setupDebugMessenger();
	app.createSurface(window.getGLFWWindow());
	app.pickPhysicalDevice();
	app.createLogicalDevice();
	app.createSwapChain(width, height);
	app.createImageViews();
	app.createDescriptorSetLayout();
	app.createGraphicsPipeline();
	//app.createTextureImage();
	app.createCommandPool();
	//app.Vulkan(monkey.GetVertices(), monkey.GetIndices());
	app.Vulkan(cube.GetVertices(), cube.GetIndices());
	app.Vulkan(cube1.GetVertices(), cube1.GetIndices());
	app.Vulkan(cube2.GetVertices(), cube2.GetIndices());
	app.Vulkan(cube3.GetVertices(), cube3.GetIndices());
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
		int width = 800, height = 600;

		Window window(width, height);
		HelloTriangleApplication app;
		Physics::PhysicsSystem m_PhysicsSystem;
		SceneManager sceneManager(&m_PhysicsSystem);
		EntityEventSystem entityEventSystem;

		// Set event bus to queue mode for better performance and to avoid issues with events being processed while entities are being destroyed
		EventBus::Get().SetImmediateMode(false); 

		//Camera
		Entity* camera = sceneManager.CreateEntity("MainCamera");
		auto transform = camera->AddComponent<TransformComponent>();
		camera->AddComponent<CameraComponent>();
		camera->AddComponent<InputComponent>();
		camera->AddComponent<MouseComponent>();


		frenet repere;


		//glm::vec3 p_0 = { -1, 1, 10 };
		//glm::vec3 p_1 = { 2, 2, 2 };
		//glm::vec3 p_2 = { 5, 5, 5 };
		//glm::vec3 p_3 = { 8, 8, 8 };
		repere.Up = { 0.f,1.f ,0.f };

		std::vector<glm::vec3> pointBase =
		{
			{  0.0f, 1.0f,  15.0f },
			{ -4.0f, 1.0f,  10.0f },  
			{  4.0f, 1.5f,   5.0f },
			{ -3.0f, 2.0f,   0.0f },
			{  3.0f, 1.5f,  -5.0f },
			{ -4.0f, 2.5f, -10.0f },
			{  0.0f, 3.0f, -15.0f },
		};

		CameraSpline cameraSpline(camera, pointBase, 100);

		glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		auto& inputManager = InputMapper::GetInstance();
		inputManager.Init(window.getGLFWWindow());

		MouseComponent::s_Window = &window;
		MouseComponent::s_Physics = &m_PhysicsSystem;

		//First Entity
		Entity* cube = sceneManager.CreateEntity("Cube");
		auto cubeTransform = cube->AddComponent<TransformComponent>();
		auto cubeRB = cube->AddComponent<RigidBodyComponent>();
		cubeTransform->SetPosition(glm::vec3(-2.0f, 0.0f, -2.0f));
		auto rb1 = m_PhysicsSystem.CreateRigidBody();
		rb1->SetKinematic(true);
		rb1->SetGravityEnabled(false);
		rb1->SetPosition(glm::vec3(-2.0f, 0.0f, -2.0f));
		rb1->SetCollider(std::make_shared<Physics::BoxCollider>(glm::vec3(1.0f)));

		cubeRB->SetRigidBody(rb1);

		//Entity 2 

		Entity* monkey = sceneManager.CreateEntity("Monkey");
		auto monkeyTransform = monkey->AddComponent<TransformComponent>();
		auto monkeyRB = monkey->AddComponent<RigidBodyComponent>();
		monkeyTransform->SetPosition(glm::vec3(2.0f, 0.0f, -2.0f));

		auto rb2 = m_PhysicsSystem.CreateRigidBody();
		rb2->SetKinematic(true);
		rb2->SetGravityEnabled(false);
		rb2->SetPosition(glm::vec3(2.0f, 0.0f, -2.0f));
		rb2->SetCollider(std::make_shared<Physics::BoxCollider>(glm::vec3(1.0f)));

		monkeyRB->SetRigidBody(rb2);


		//Remove this after MeshComponent is done
		app.sceneObjects.push_back(std::make_pair(0, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -2.0f))));
		app.sceneObjects.push_back(std::make_pair(1, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -2.0f))));

		launchVulkan(app, window, width, height);

		app.camTest = camera;


		while (!window.WindowClosed())
		{
			window.PollEvent();
			inputManager.Update();
			/*float currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;*/

			//inputManager.Update();

			auto Cam = app.camTest->GetComponent<InputComponent>();
			Cam->Update(0.16f);
			auto CamPos = app.camTest->GetComponent<TransformComponent>();
			

			//Mouse input is handled in the MouseComponent, so we update it here
			if (auto mouseInputs = app.camTest->GetComponent<MouseComponent>())
			{
				mouseInputs->Update(0.16f);
			}
			cameraSpline.Update();

			int width = 0, height = 0;
			window.GetFramebufferSize(width, height);
			while (width == 0 || height == 0)
			{
				window.GetFramebufferSize(width, height);
				window.WaitEvents();
			}

			app.drawFrame(width, height);

			EventBus::Get().ProcessEvent();
			sceneManager.CleanupDestroyedEntities();

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