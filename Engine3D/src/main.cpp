#include "Renderer.h"
#include "BaseComponent.h"
#include "window.h"
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


		Entity* camera = new Entity("mainCamera");
		auto transform = camera->AddComponent<TransformComponent>();
		camera->AddComponent<CameraComponent>();
		camera->AddComponent<InputComponent>();
		camera->AddComponent<MouseComponent>();


		frenet repere;


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


		input::init(window.getGLFWWindow());
		auto& inputManager = InputMapper::GetInstance();
		inputManager.Init(window.getGLFWWindow());

		app.sceneObjects.push_back(std::make_pair(0, glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 5.0f))));
		app.sceneObjects.push_back(std::make_pair(1, glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f))));
		app.sceneObjects.push_back(std::make_pair(2, glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, -5.0f))));
		app.sceneObjects.push_back(std::make_pair(3, glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, -10.0f))));


		launchVulkan(app, window, width, height);
		app.camTest = camera;


		static auto previous = std::chrono::high_resolution_clock::now();

		while (!window.WindowClosed())
		{

			auto current = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration<float, std::milli>(current - previous).count();

			previous = current;


			window.PollEvent();
			inputManager.Update();
			auto Cam = app.camTest->GetComponent<InputComponent>();
			Cam->Update(elapsed);
			auto CamPos = app.camTest->GetComponent<TransformComponent>();
			

			cameraSpline.Update(elapsed);

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
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
}