#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Entity.h"

struct frenet
{
	glm::vec3 Up;

	glm::vec3 Tangent;
	glm::vec3 Normal;
	glm::vec3 Binormal;
};


class CameraSpline
{
public:

	CameraSpline(Entity* camera, std::vector<glm::vec3>& pointBase, int pointBeetwen = 10)
		: m_camera(camera), m_pointBeetwen(pointBeetwen), repere{ {0.f, 1.f, 0.f} }
	{
		m_transform = m_camera->GetComponent<TransformComponent>();
		int pointBaseSize = (int)pointBase.size();

		for (int i = 0; i < pointBaseSize; ++i)
		{
			glm::vec3 p0 = pointBase[(i - 1 + pointBaseSize) % pointBaseSize];
			glm::vec3 p1 = pointBase[(i) % pointBaseSize];
			glm::vec3 p2 = pointBase[(i + 1 + pointBaseSize) % pointBaseSize];
			glm::vec3 p3 = pointBase[(i + 2 + pointBaseSize) % pointBaseSize];

			for (int j = 0; j < m_pointBeetwen; ++j)
			{
				float t = (float)j / float(m_pointBeetwen);

				glm::vec3 point = spline(t, p0, p1, p2, p3);
				m_points.push_back(point);
			}

		}
		// Dans le constructeur de CameraSpline, après le calcul des points
		for (const auto& point : m_points)
			std::cout << "Point: " << point.x << ", " << point.y << ", " << point.z << std::endl;

	}
	
	void Update()
	{
		static int index = 0;
		static int direction = 1;
		static int frameCount = 0;
		const int frameCountMax = 10;

		frameCount++;
		if (frameCount >= frameCountMax)
		{
			frameCount = 0;
			index += direction;

			if (index >= (int)m_points.size() - 1)
			{
				index = (int)m_points.size() - 1;
				direction = -1;
			}
			else if (index <= 0)
			{
				index = 0;
				direction = 1;
			}
		}

		if (!m_points.empty() && index < m_points.size())
		{
			glm::vec3 currentPos = m_points[index];
			glm::vec3 nextPos = m_points[(index + 1) % (int)m_points.size()];
			m_transform->SetPosition(currentPos);

			glm::vec3 distVec = nextPos - currentPos;
			float distance = glm::length(distVec);

			if (distance > 0.0001f)
			{
				repere.Tangent = glm::normalize(distVec);
				glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
				if (std::abs(glm::dot(up, repere.Tangent)) > 0.99f)
					up = glm::vec3(1.0f, 0.0f, 0.0f);

				repere.Binormal = glm::normalize(glm::cross(repere.Tangent, up));
				repere.Normal = glm::cross(repere.Binormal, repere.Tangent);

				glm::mat4 rotation = glm::mat4(1);
				rotation[0] = glm::vec4(repere.Binormal, 0);
				rotation[1] = glm::vec4(repere.Normal, 0);
				rotation[2] = glm::vec4(-repere.Tangent, 0);
				m_transform->SetRotation(rotation);

				glm::mat4 view = glm::lookAt(
					currentPos,
					currentPos + repere.Tangent,
					repere.Normal
				);
			}
		}
	}



	glm::vec3 getCurrentPosition() const
	{
		return m_points[m_index];
	}
	int getCurrentIndex() const
	{
		return m_index;
	}
	bool IsFinished() const
	{
		return false;
	}

private:
	Entity* m_camera = nullptr;
	TransformComponent* m_transform = nullptr;
	frenet repere;

	glm::vec3 spline(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		glm::vec3 m0 = (p2 - p0) * glm::vec3(0.5, 0.5, 0.5);
		glm::vec3 m1 = (p3 - p1) * glm::vec3(0.5, 0.5, 0.5);

		float carre = t * t;
		float cube = carre * t;

		glm::vec3 p;

		return p = (2 * cube - 3 * carre + 1) * p1 + (cube - 2 * carre + t) * m0 + (-2 * cube + 3 * carre) * p2 + (cube - carre) * m1;
	}

	std::vector<glm::vec3> m_points;

	int m_index = 0;
	int m_direction = 1;
	int m_pointBeetwen;
	float m_timer = 0;
};