#pragma once
#include "glmConfig.h"
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Entity.h"
#include "InputMapper.h"
#include "window.h"
#include "Physics.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EventBus.h"
#include "EventListener.h"

class TransformComponent : public Component
{
private: 
	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	//Cached Transformation Matrix
	mutable glm::mat4 transformationMatrix = glm::mat4(1.0f);
	mutable bool transformDirty = true;

public:
	void SetPosition(const glm::vec3& pos)
	{
		position = pos;
		transformDirty = true;
	}

	void SetRotation(const glm::quat& rot)
	{
		rotation = rot;
		transformDirty = true;
	}

	void SetScale(const glm::vec3& s)
	{
		scale = s;
		transformDirty = true;
	}

	const glm::vec3& GetPosition() const { return position; }
	const glm::quat& GetRotation() const { return rotation; }
	const glm::vec3& GetScale() const { return scale; }

	glm::mat4 GetTransformMatrix() const
	{
		if (transformDirty)
		{
			//Calcul transformation matrix
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
			glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

			transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
			transformDirty = false;
		}
		return transformationMatrix;
	}
};

class MeshComponent : public Component
{
/*private:
	Mesh* mesh = nullptr;
	Material* material = nullptr;
public:
	MeshComponent(Mesh* m, Material* mat) : mesh(m), material(mat) {}

	void SetMesh(Mesh* m) { mesh = m; }
	void SetMaterial(Material* mat) { material = mat; }

	Mesh* GetMesh() const { return mesh; }
	Material* GetMaterial() const { return material; }

	void Render() override
	{
		if (!mesh || !material) return;

		auto transform = GetOwner()->GetComponent<TransformComponent>();
		if (!transform) return;

		material->Bind();
		material->SetUniform("modelMatrix", transform->GetTransformMatrix());
		mesh->Render();
	}*/
};

class CameraComponent : public Component
{
private:
	float fieldOfView = 45.0f;
	float aspectRatio = 16.0f / 9.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	mutable glm::mat4 projectionMatrix = glm::mat4(1.0f);
	mutable bool projectionDirty = true;

public:
	void SetPerspective(float fov, float aspect, float Near, float Far)
	{
		fieldOfView = fov;
		aspectRatio = aspect;
		nearPlane = Near;
		farPlane = Far;
		projectionDirty = true;
	}

	glm::mat4 GetViewMatrix() const
	{
		if (auto transform = GetOwner()->GetComponent<TransformComponent>(); transform)
		{
			glm::vec3 position = transform->GetPosition();
			glm::quat rotation = transform->GetRotation();

			glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);

			glm::vec3 up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

			return glm::lookAt(position, position + forward, up);
		}
		return glm::mat4(1.0f);
	}

	glm::mat4 GetProjectionMatrix() const {
		if (projectionDirty) {
			projectionMatrix = glm::perspective(
				glm::radians(fieldOfView),
				aspectRatio,
				nearPlane,
				farPlane
			);
			projectionDirty = false;
		}
		return projectionMatrix;
	}

	static void BuildMouseRay(double mouseX, double mouseY,int screenW, int screenH,const CameraComponent& cam,const TransformComponent& camTransform,glm::vec3& outOrigin,glm::vec3& outDir)
	{
		float x = static_cast<float>(mouseX);
		float y = static_cast<float>(screenH - mouseY);

		glm::vec4 viewport(0.f, 0.f, static_cast<float>(screenW), static_cast<float>(screenH));

		glm::mat4 view = cam.GetViewMatrix();
		glm::mat4 proj = cam.GetProjectionMatrix();

		// Vulkan depth: near = 0, far = 1
		glm::vec3 nearP = glm::unProjectZO(glm::vec3(x, y, 0.0f), view, proj, viewport);
		glm::vec3 farP = glm::unProjectZO(glm::vec3(x, y, 1.0f), view, proj, viewport);

		outOrigin = camTransform.GetPosition();              
		outDir = glm::normalize(farP - nearP);
	}
};

class LightComponent : public Component
{
	//entite scenique // rendering
	// couleur intensité, radius etc // tri pour savoir ce qu'elle doit faire
private:

	enum Type
	{
		directional,
		point,
		spot,
		Ambient //skybox light
	};
	Type lightType = Type::Ambient;
	glm::vec3 lightColor; //RGB
	float lightIntensity = 100.0f;
	float lightRadius = 50.0f;

public:
	void setLight(Type type, glm::vec3 color, const float intensity, const float radius)
	{
		//Create good class based on type ?
		switch (type)
		{
		case Type::directional:
			break;
		case Type::spot:
			break;
		case Type::point:
			break;
		default:
			break;
		}
	}

};

class RigidBodyComponent : public Component
{
private:
	std::shared_ptr<Physics::RigidBody> body = nullptr;
public:
	void SetRigidBody(const std::shared_ptr<Physics::RigidBody>& rb)
	{
		body = rb;
		if (body && GetOwner())
		{
			body->SetOwner(GetOwner());
		}
	}
	std::shared_ptr<Physics::RigidBody> GetRigidBody() const { return body; }

	void Update(float deltatime) override
	{
		if (!body) return;
		auto transform = GetOwner()->GetComponent<TransformComponent>();
		if (!transform) return;
		transform->SetPosition(body->GetPosition());
		transform->SetRotation(body->GetRotation());
	}

	void OnDestroy() override
	{
		body = nullptr; // Let the physics system handle the actual destruction of the rigid body
	}
};

class SoundComponent : public Component
{
	// base sound parameters
};
class SoundEmetteur : public SoundComponent
{
	//sound parameters
};

class SoundListener : public SoundComponent //This could also be something in camera need to see later
{
	//listener parameters
};

class InputComponent : public Component {

public:

	void Update(float dt) override {
		float speed = 2.0f;
		auto transform = GetOwner()->GetComponent<TransformComponent>();
		if (!transform) return;

		glm::vec3 position = transform->GetPosition();
		glm::quat rotation = transform->GetRotation();

		// Get input values
		float horizontal = InputMapper::GetInstance().GetAxis("Horizontal");
		float vertical = InputMapper::GetInstance().GetAxis("Vertical");
		float forward = InputMapper::GetInstance().GetAxis("Forward");

		// Calculate movement
		glm::vec3 moveDirection =
			(rotation * glm::vec3(1.0f, 0.0f, 0.0f)) * horizontal +
			(rotation * glm::vec3(0.0f, 1.0f, 0.0f)) * vertical +
			(rotation * glm::vec3(0.0f, 0.0f, -1.0f)) * forward;

		position += moveDirection * speed * dt;
		transform->SetPosition(position);
	}
};

class MouseComponent : public Component, public EventListener{
	//mouse parameters
public:
	inline static Physics::PhysicsSystem* s_Physics = nullptr;
	inline static Window* s_Window = nullptr;

	void MouseInit(Physics::PhysicsSystem* physics, Window* window)
	{
		s_Physics = physics;
		s_Window = window;

		EventBus::Get().AddListener(this);
	}

	void OnEvent(const Event& event) override
	{
		//Handle mouse events if needed
	}

	void Update(float deltatime) override
	{
		if (!InputMapper::GetInstance().isButtonPressed("Shoot")) return;
		if (!s_Window || !s_Physics) return;

		GLFWwindow* win = InputMapper::GetInstance().m_window;

		//Mouse position in WINDOW coordinates
		double mxWin = 0.0, myWin = 0.0;
		glfwGetCursorPos(win, &mxWin, &myWin);

		int winW = 0, winH = 0;
		glfwGetWindowSize(win, &winW, &winH);

		int fbW = 0, fbH = 0;
		s_Window->GetFramebufferSize(fbW, fbH);

		if (winW <= 0 || winH <= 0 || fbW <= 0 || fbH <= 0) return;

		//Convert mouse coords -> FRAMEBUFFER coordinates
		const double sx = static_cast<double>(fbW) / static_cast<double>(winW);
		const double sy = static_cast<double>(fbH) / static_cast<double>(winH);

		const double mx = mxWin * sx;
		const double my = myWin * sy;

		auto cam = GetOwner()->GetComponent<CameraComponent>();
		auto cameraTransform = GetOwner()->GetComponent<TransformComponent>();
		if (!cam || !cameraTransform) return;

		glm::vec3 origin, direction;
		CameraComponent::BuildMouseRay(mx, my, fbW, fbH, *cam, *cameraTransform, origin, direction);
		glm::vec3 forward = cameraTransform->GetRotation() * glm::vec3(0, 0, -1);

		Physics::RaycastHit hit;

		if (s_Physics->Raycast(origin, direction, 1000.0f, hit))
		{
			std::printf("HIT dist=%.3f\n", hit.distance);
			if (hit.body)
			{
				Entity* hitEntity = hit.body->GetOwner();
				if (hitEntity)
				{
					HitEvent event(hitEntity);
					std::printf("Publishing HitEvent\n");
					EventBus::Get().PublishEvent(event);

				}
				else
				{
					std::printf("Hit a body with no owner entity\n");
				}
			}
		}
		else
		{
			std::printf("Nothing hit\n");
		}
	}

	void OnDestroy() override
	{
		EventBus::Get().RemoveListener(this);
	}
};

class CanvasComponent : public Component //UI box
{
	//UI parameters
};