#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

//namespace Engine
//{
	namespace Physics
	{
		struct RaycastHit;

		enum class ColliderType
		{
			Box,
			Sphere,
			Capsule,
			Mesh
		};

		class Collider
		{
		public:
			virtual ~Collider() = default;
			virtual ColliderType GetType() const = 0;

			void SetOffSet(const glm::vec3& offset) { m_Offset = offset; }
			const glm::vec3& GetOffSet() const { return m_Offset; }

		protected:
			glm::vec3 m_Offset = glm::vec3(0.0f);
		};

		class BoxCollider : public Collider
		{
		public:
			BoxCollider(const glm::vec3& halfExtents) : m_HalfExtents(halfExtents) {}

			ColliderType GetType() const override { return ColliderType::Box; }

			const glm::vec3& GetHalfExtents() const { return m_HalfExtents; }
			void SetHalfExtents(const glm::vec3& halfExtents) { m_HalfExtents = halfExtents; }

		private:
			glm::vec3 m_HalfExtents;
		};

		class SphereCollider : public Collider
		{
		public:
			SphereCollider(float radius) : m_Radius(radius) {}
			ColliderType GetType() const override { return ColliderType::Sphere; }
			float GetRadius() const { return m_Radius; }
			void SetRadius(float radius) { m_Radius = radius; }
		private:
			float m_Radius;
		};

		class RigidBody
		{
		public:
			RigidBody();
			~RigidBody();

			//Kinematic State
			void SetPosition(const glm::vec3& position) { m_Position = position; }
			void SetRotation(const glm::quat& rotation) { m_Rotation = rotation; }
			void SetLinearVelocity(const glm::vec3& velocity) { m_LinearVelocity = velocity; }
			void SetAngularVelocity(const glm::vec3& velocity) { m_AngularVelocity = velocity; }

			const glm::vec3& GetPosition() const { return m_Position; }
			const glm::quat& GetRotation() const { return m_Rotation; }
			const glm::vec3& GetLinearVelocity() const { return m_LinearVelocity; }
			const glm::vec3& GetAngularVelocity() const { return m_AngularVelocity; }

			//Physical Properties

			void SetMass(float mass);
			float GetMass() const { return m_Mass; }
			float GetInverseMass() const { return m_InverseMass; }

			void SetRestitution(float restitution) { m_Restitution = restitution; }
			float GetRestitution() const { return m_Restitution; }

			void SetFriction(float friction) { m_Friction = friction; }
			float GetFriction() const { return m_Friction; }

			//Collider Management

			void SetCollider(std::shared_ptr<Collider> collider) { m_Collider = collider; }
			std::shared_ptr<Collider> GetCollider() const { return m_Collider; }

			//Forces and Impulses

			void ApplyForce(const glm::vec3& force);
			void ApplyTorque(const glm::vec3& torque);
			void ApplyImpulse(const glm::vec3& impulse);
			void ApplyTorqueImpulse(const glm::vec3& torqueImpulse);

			//Simulation flags

			void SetKinematic(bool kinematic) { m_IsKinematic = kinematic; }
			bool IsKinematic() const { return m_IsKinematic; }

			void SetGravityEnabled(bool enabled) { m_UseGravity = enabled; }
			bool IsGravityEnabled() const { return m_UseGravity; }

		private:

			//kinematic state
			glm::vec3 m_Position = glm::vec3(0.0f);
			glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			glm::vec3 m_LinearVelocity = glm::vec3(0.0f);
			glm::vec3 m_AngularVelocity = glm::vec3(0.0f);

			//Forces
			glm::vec3 m_AccumulatedForces = glm::vec3(0.0f);
			glm::vec3 m_AccumulatedTorques = glm::vec3(0.0f);

			//Physical properties
			float m_Mass = 1.0f;
			float m_InverseMass = 1.0f;
			glm::mat3 m_InertiaTensor = glm::mat3(1.0f);
			glm::mat3 m_InverseInertiaTensor = glm::mat3(1.0f);
			float m_Restitution = 0.5f;
			float m_Friction = 0.5f;

			//Collisions
			std::shared_ptr<Collider> m_Collider;

			//Flags
			bool m_IsKinematic = false;
			bool m_UseGravity = true;

			//Update inertia tensor based on mass and collider
			void UpdateInnertiaTensor();
			
			friend class PhysicsSystem;
		};

		struct CollisionInfo
		{
			std::shared_ptr<RigidBody> bodyA;
			std::shared_ptr<RigidBody> bodyB;
			glm::vec3 contactPoint;
			glm::vec3 normal;
			float penetrationDepth;
		};

		class PhysicsSystem
		{
		public:
			//PhysicsSystem();
			//~PhysicsSystem();

			//void Initialize();
			//void Shutdown();

			//Update physics simulation
			void Update(float deltaTime);

			//RigidBody management
			std::shared_ptr<RigidBody> CreateRigidBody();
			void DestroyRigidBody(const std::shared_ptr<RigidBody>& body);

			//World Setting
			void SetGravity(const glm::vec3& gravity) { m_Gravity = gravity; }
			const glm::vec3& GetGravity() const { return m_Gravity; }

			//Collisions Detection
			bool Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RaycastHit& hit);

			

		private:
			std::vector<std::shared_ptr<RigidBody>> m_RigidBodies;
			glm::vec3 m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f);

			//Simulation steps
			void IntegrateForces(RigidBody& body, float deltaTime);
			void IntegrateVelocities(RigidBody& body, float deltaTime);

			//Collision detection and response
			void DetectCollisions(std::vector<CollisionInfo>& collisions);
			void ResolveCollisions(std::vector<CollisionInfo>& collisions);

			//Helper functions for collision detection
			bool CheckCollisions(const RigidBody& bodyA, const RigidBody& bodyB, CollisionInfo& collision);
			bool SphereVSSphere(const RigidBody& bodyA, const RigidBody& bodyB, CollisionInfo& collision);
			bool BoxVSBox(const RigidBody& bodyA, const RigidBody& bodyB, CollisionInfo& collision);
			//bool SphereVSBox(const RigidBody& bodyA, const RigidBody& bodyB, CollisionInfo& collision);
		};

		struct RaycastHit
		{
			glm::vec3 point;
			glm::vec3 normal;
			float distance;
			std::shared_ptr<RigidBody> body;
		};
	}
	
//}
