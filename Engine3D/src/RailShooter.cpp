#include "World.h"

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneManager.h"
#include "Entity.h"
#include "BaseComponent.h"
#include "Engine.hpp"
#include "MeshComponent.h"

void RailShooter::SettingWorld(SceneManager* scenemanager) 
{
	m_scenemanager = scenemanager;

	m_Camera = scenemanager->CreateEntity("MainCamera");
	auto transform = m_Camera->AddComponent<TransformComponent>();
	m_Camera->AddComponent<CameraComponent>();
	m_Camera->AddComponent<InputComponent>();
	m_Camera->AddComponent<MouseComponent>();

	frenet repere;
	repere.Up = { 0.0f,1.0f,0.0f };
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

	m_CameraSpline = std::make_unique<CameraSpline>(m_Camera, pointBase, 100);


	MouseComponent::s_Window = m_ObserverEngine->GetWindow().get();
	MouseComponent::s_Physics = m_ObserverEngine->GetPhysicSystem().get();

	//First Entity
	m_cube = scenemanager->CreateEntity("Cube");
	m_cube->AddComponent<MeshComponent>("Assets/box.obj");
	auto cubeTransform = m_cube->AddComponent<TransformComponent>();
	auto cubeRB = m_cube->AddComponent<RigidBodyComponent>();
	cubeTransform->SetPosition(glm::vec3(-2.0f, 0.0f, -2.0f));
	auto rb1 = m_ObserverEngine->GetPhysicSystem()->CreateRigidBody();
	rb1->SetKinematic(true);
	rb1->SetGravityEnabled(false);
	rb1->SetPosition(glm::vec3(-2.0f, 0.0f, -2.0f));
	rb1->SetCollider(std::make_shared<Physics::BoxCollider>(glm::vec3(1.0f)));
	cubeRB->SetRigidBody(rb1);

	//Entity 2 
	m_monkey = scenemanager->CreateEntity("Monkey");
	m_monkey->AddComponent<MeshComponent>("Assets/monkey.obj");
	auto monkeyTransform = m_monkey->AddComponent<TransformComponent>();
	auto monkeyRB = m_monkey->AddComponent<RigidBodyComponent>();
	monkeyTransform->SetPosition(glm::vec3(2.0f, 0.0f, -2.0f));
	auto rb2 = m_ObserverEngine->GetPhysicSystem()->CreateRigidBody();
	rb2->SetKinematic(true);
	rb2->SetGravityEnabled(false);
	rb2->SetPosition(glm::vec3(2.0f, 0.0f, -2.0f));
	rb2->SetCollider(std::make_shared<Physics::BoxCollider>(glm::vec3(1.0f)));
	monkeyRB->SetRigidBody(rb2);

	//TODO : Jsp se que vous vouliez en faire je le laisse ici
	// 		//Remove this after MeshComponent is done
// 		app.sceneObjects.push_back(std::make_pair(0, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -2.0f))));
// 		app.sceneObjects.push_back(std::make_pair(1, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -2.0f))));
		// 		launchVulkan(app, window, width, height);

// 		app.camTest = camera;
}

void RailShooter::Update(float elapsed) 
{
	// 			auto Cam = app.camTest->GetComponent<InputComponent>();
// 			Cam->Update(0.16f);
// 			auto CamPos = app.camTest->GetComponent<TransformComponent>();

		//Mouse input is handled in the MouseComponent, so we update it here

		/*if (auto mouseInputs = app.camTest->GetComponent<MouseComponent>())
		{
			mouseInputs->Update(elapsed);
		}*/
	m_CameraSpline->Update();
}

void RailShooter::KillWorld() 
{
	m_scenemanager->CleanupDestroyedEntities();
}