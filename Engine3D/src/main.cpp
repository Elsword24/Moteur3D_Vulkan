#include "Renderer.h"
#include "BaseComponent.h"
#include "window.h"
#include "InputMapper.h"
#include "spline.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void launchVulkan( HelloTriangleApplication& app, Window* window, int width, int height )
{
	Mesh monkey;
	monkey.LoadModel("Assets/monkey.obj");

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
		Window window(WIDTH, HEIGHT);
		HelloTriangleApplication app;


		Entity* camera = new Entity("mainCamera");
		auto transform = camera->AddComponent<TransformComponent>();
		camera->AddComponent<CameraComponent>();
		camera->AddComponent<InputComponent>();
		camera->AddComponent<MouseComponent>();



		frenet repere;


		glm::vec3 p_0 = { -1, 1, 10 };
		glm::vec3 p_1 = { 2, 2, 2 };
		glm::vec3 p_2 = { 5, 5, 5 };
		glm::vec3 p_3 = { 8, 8, 8 };
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

		//app.initWindow();

		//window.initWindow();
		input::init(window.getGLFWWindow());
		app.sceneObjects.push_back(std::make_pair(1, glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 5.0f))));
		app.sceneObjects.push_back(std::make_pair(2, glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f))));
		app.sceneObjects.push_back(std::make_pair(3, glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, -5.0f))));
		app.sceneObjects.push_back(std::make_pair(4, glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, -10.0f))));
		int width = 1920, height = 1080;

		launchVulkan(app, &window, width, height);
		app.camTest = camera;

		float lastFrame = 0.0f;

		while (!window.WindowClosed())
		{

			float currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			InputMapper::GetInstance().Update();
			//inputManager.Update();

			auto Cam = app.camTest->GetComponent<InputComponent>();
			Cam->Update(0.16f);
			auto CamPos = app.camTest->GetComponent<TransformComponent>();
			

			cameraSpline.Update();

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