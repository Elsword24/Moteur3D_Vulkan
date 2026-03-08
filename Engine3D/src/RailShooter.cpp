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
	transformCamera = m_Camera->GetComponent<TransformComponent>();

	frenet repere;
	repere.Up = { 0.0f,1.0f,0.0f };

	std::vector<glm::vec3> pointBase = {
		{  0.0f, 1.0f,  7.0f },
		{  0.0f, 1.0f,  5.0f },
		{ -1.0f, 1.0f,  2.5f },
		{ -2.0f, 1.0f,  0.0f },
		{ -1.0f, 1.0f, -2.5f },
		{  1.0f, 1.0f, -4.0f },
		{  2.0f, 1.0f, -5.5f },
		{  1.0f, 1.0f, -6.5f },
		{  0.0f, 1.0f, -7.0f },
		{ -0.5f, 1.0f, -7.5f },
		{  0.0f, 1.0f, -7.5f },
		{  0.0f, 1.0f, -7.5f }
	};



	MouseComponent::s_Window = m_ObserverEngine->GetWindow().get();
	MouseComponent::s_Physics = m_ObserverEngine->GetPhysicSystem().get();


	m_sol = scenemanager->CreateEntity("Cube");
	m_sol->AddComponent<MeshComponent>("Assets/box.obj");
	auto solTransform = m_sol->AddComponent<TransformComponent>();
	solTransform->SetPosition(glm::vec3(0,0,0));
	solTransform->SetScale(glm::vec3(15,0.5,15));



	//Entity 2 
	m_monkey = scenemanager->CreateEntity("Monkey");
	m_monkey->AddComponent<MeshComponent>("Assets/monkey.obj");
	monkeyTransform = m_monkey->AddComponent<TransformComponent>();
	auto monkeyRB = m_monkey->AddComponent<RigidBodyComponent>();
	monkeyTransform->SetPosition(glm::vec3(2.0f, 0.0f, -2.0f));
	auto rb2 = m_ObserverEngine->GetPhysicSystem()->CreateRigidBody();
	rb2->SetKinematic(true);
	rb2->SetGravityEnabled(false);
	rb2->SetPosition(glm::vec3(2.0f, 0.0f, -2.0f));
	rb2->SetCollider(std::make_shared<Physics::BoxCollider>(glm::vec3(1.0f)));
	monkeyRB->SetRigidBody(rb2);

	//for (int i = 0; i < 5; ++i)
	//{
	//	Entity* enemy = scenemanager->CreateEntity("Enemy " + std::to_string(i));
	//	auto mesh = enemy->AddComponent<MeshComponent>("Assets/box.obj");
	//	auto transformEnemy = enemy->AddComponent<TransformComponent>();
	//	transformEnemy->SetPosition(glm::vec3{i*2,2,-i*3});
	//	m_enemies.push_back(enemy);
	//}


	m_CameraSpline = std::make_unique<CameraSpline>(m_Camera, pointBase, 100);

}

void RailShooter::Update(float elapsed)
{
	m_scenemanager->Update(elapsed);
	m_CameraSpline->Update(elapsed);

	glm::vec3 camPos = transformCamera->GetPosition();
	glm::quat camRot = transformCamera->GetRotation();
	glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 forward = camRot * worldForward;
	float distanceDevant = 10.0f;
	glm::vec3 monkeyPos = camPos + (forward * distanceDevant);
	monkeyPos.y -= 1.5f;

	monkeyTransform->SetPosition(monkeyPos);
	monkeyTransform->SetRotation(camRot);

	auto monkeyRB = m_monkey->GetComponent<RigidBodyComponent>();
	if (monkeyRB && monkeyRB->GetRigidBody()) {
		monkeyRB->GetRigidBody()->SetPosition(monkeyPos);
	}

	for (const auto& enemy : m_enemies)
	{
		auto enemyRB = enemy->GetComponent<RigidBodyComponent>();
		if (enemyRB && enemyRB->GetRigidBody()) {
			enemyRB->GetRigidBody()->SetPosition(monkeyPos);
		}
	}
}

void RailShooter::KillWorld() 
{
	m_scenemanager->CleanupDestroyedEntities();

}