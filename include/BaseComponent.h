#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Entity.h"

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

/*class MeshComponent : public Component
{
private:
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
	}
};*/

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
	void SetPerspective(float fov, float aspect, float near, float far)
	{
		fieldOfView = fov;
		aspectRatio = aspect;
		nearPlane = near;
		farPlane = far;
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
};

class LightComponent : public Component
{
	//entité scénique // rendering
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
	//physics parameters
};

class SoundComponent : public Component {};
class SoundEmetteur : public SoundComponent
{
	//sound parameters
};

class SoundListener : public SoundComponent //This could also be something in camera need to see later
{
	//listener parameters
};

class InputComponent : public Component //based on Valentine code
{
	//input parameters
};

class CanvasComponent : public Component //UI box
{
	//UI parameters
};
//Maybe do another family of component for the colliders components, like BoxColliderComponent, SphereColliderComponent, etc. that would be used by the physics system to detect collisions and trigger events.

