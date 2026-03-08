#ifndef WORLD_H
#define WORLD_H
#pragma once


#include <memory>
#include "spline.h"

class Entity;
class EngineQVY;
class SceneManager;

//class CameraSpline;


class IWorld
{
private:
	SceneManager* m_SceneManager = nullptr;
public:
	IWorld() = default;
	virtual ~IWorld() = default;

	virtual void SettingWorld(SceneManager *scenemanager) = 0;
	virtual void Update(float elapsed) = 0;
	virtual void KillWorld() = 0;
};




class RailShooter : public IWorld
{
private:
	struct Bullet
	{
		Entity* bulletEntity;
		glm::vec3 direction;
		float speed = 30.0f;
		float timeLife = 2.5f;
	};

	int nbrBulletMax = 100;
	Entity* m_cube = nullptr;
	std::vector<Entity*> m_enemies;
	std::vector<Bullet> m_bullets;
	Entity* m_Camera = nullptr;
	Entity* m_monkey = nullptr;
	TransformComponent* transformCamera;
	TransformComponent* monkeyTransform;
	EngineQVY* m_ObserverEngine = nullptr;
	SceneManager* m_scenemanager = nullptr;
	std::unique_ptr<CameraSpline> m_CameraSpline;
	float m_shootTime = 0.0f;
	float m_shootFrameRite = 0.2f;

public:
	RailShooter(EngineQVY* observerEngine)
		:m_ObserverEngine(observerEngine)
	{

	}

	~RailShooter() = default;
	void SettingWorld(SceneManager* scenemanager) override;

	void Update(float elapsed) override;

	void KillWorld() override;
};

#endif

