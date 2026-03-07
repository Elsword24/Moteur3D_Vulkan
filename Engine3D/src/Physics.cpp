#include "Physics.h"
#include <algorithm>
#include <cmath>

static bool RayVSSphere(const glm::vec3& o, const glm::vec3& d, float maxDist, const glm::vec3& center, float radius, float& outT, glm::vec3& outNormal)
{
	glm::vec3 m = o - center;
	float b = glm::dot(m, d);
	float c = glm::dot(m, m) - radius * radius;

	//Si le rayon pointe a l'opposé
	if (c > 0.0f && b > 0.0f) return false;

	float discrete = b * b - c;
	if (discrete < 0.0f) return false;

	float t = -b - std::sqrt(discrete);
	if (t < 0.0f) t = 0.0f; //Origin is in sphere

	if (t > maxDist) return false;

	outT = t;
	glm::vec3 p = o + d * t;
	outNormal = glm::normalize(p - center);
	return true;
}

static bool RayVsAABB(const glm::vec3& o, const glm::vec3& d, float maxDist, const glm::vec3& bmin, const glm::vec3& bmax, float& outT, glm::vec3& outNormal)
{
	float tMin = 0.0f;
	float tMax = maxDist;
	glm::vec3 n(0.0f);

	for (int axis=0;axis <3; axis++)
	{
		float oA = o[axis];
		float dA = d[axis];
		float minA = bmin[axis];
		float maxA = bmax[axis];

		if (std::abs(dA) <1e-8f)
		{
			//Rayon parallèle 
			if (oA < minA || oA > maxA) return false;
		} else
		{
			float invD = 1.0f / dA;
			float t1 = (minA - oA) * invD;
			float t2 = (maxA - oA) * invD;

			float sign = 1.0f;
			if (t1>t2)
			{
				std::swap(t1, t2);
				sign = -1.0f;
			}
			if (t1 > tMin)
			{
				tMin = t1;
				n = glm::vec3(0.0f);
				n[axis] = -sign; //normale de la face 
			}
			tMax = std::min(tMax, t2);
			if (tMin > tMax) return false;
		}
	}
	outT = tMin;
	outNormal = n;
	return true;
}

//namespace Engine {
	namespace Physics
	{

		RigidBody::RigidBody() = default;
		RigidBody::~RigidBody() = default;

		void PhysicsSystem::Update(float deltatime)
		{
			//fixed timestep for stability
			const float fixedTimeStep = 1.0f / 60.0f;

			//Accumulate forces
			for (auto& body : m_RigidBodies)
			{
				if (!body->IsKinematic() && body->IsGravityEnabled())
				{
					body->m_AccumulatedForces += m_Gravity * body->m_Mass;
				}
			}

			//Intergrate forces
			for (auto& body : m_RigidBodies)
			{
				if (!body->IsKinematic())
				{
					IntegrateForces(*body, fixedTimeStep);
				}
			}

			//Detect and resolve collisions
			std::vector<CollisionInfo> collisions;
			DetectCollisions(collisions);
			ResolveCollisions(collisions);

			//Intergrate velocities
			for (auto& body : m_RigidBodies)
			{
				if (!body->IsKinematic())
				{
					IntegrateVelocities(*body, fixedTimeStep);
				}
			}

			//Clear Accumulated forces
			for (auto& body : m_RigidBodies)
			{
				body->m_AccumulatedForces = glm::vec3(0.0f);
				body->m_AccumulatedTorques = glm::vec3(0.0f);
			}
		}

		std::shared_ptr<RigidBody> PhysicsSystem::CreateRigidBody()
	{
			auto body = std::make_shared<RigidBody>();
			m_RigidBodies.push_back(body);
			return body;
	}

		void PhysicsSystem::DestroyRigidBody(const std::shared_ptr<RigidBody>& body)
	{
		if (!body) return;

		auto it = std::remove(m_RigidBodies.begin(), m_RigidBodies.end(), body);
		m_RigidBodies.erase(it, m_RigidBodies.end());
	}

		void PhysicsSystem::IntegrateForces(RigidBody& body, float deltaTime) 
		{
			//Update linear velocity
			body.m_LinearVelocity += (body.m_AccumulatedForces * body.m_InverseMass) * deltaTime;

			//Update Angular velocity
			body.m_AngularVelocity += glm::vec3(body.m_InverseInertiaTensor * glm::vec4(body.m_AccumulatedTorques, 0.0f)) * deltaTime;

			//Apply Damping
			const float linearDamping = 0.01f;
			const float angularDamping = 0.01f;
			body.m_LinearVelocity *= (1.0f - linearDamping);
			body.m_AngularVelocity *= (1.0f - angularDamping);
		}

		void PhysicsSystem::IntegrateVelocities(RigidBody& body, float deltaTime) 
		{
			//Update position
			body.m_Position += body.m_LinearVelocity * deltaTime;

			//Update rotation
			glm::quat angularVelocityQuat(0.0f, body.m_AngularVelocity.x, body.m_AngularVelocity.y, body.m_AngularVelocity.z);
			body.m_Rotation += (angularVelocityQuat * body.m_Rotation) * 0.5f * deltaTime;
			body.m_Rotation = glm::normalize(body.m_Rotation);
		}

		void PhysicsSystem::DetectCollisions(std::vector<CollisionInfo>& collisions)
		{
			//Simple O(n²) collision detection
			for (size_t i=0; i< m_RigidBodies.size(); i++)
			{
				for (size_t j =i +1; j < m_RigidBodies.size(); j++)
				{
					auto& bodyA = m_RigidBodies[i];
					auto& bodyB = m_RigidBodies[j];

					//skip if both bodies are kinematic
					if (bodyA->IsKinematic() && bodyB->IsKinematic())
					{
						continue;
					}

					//skip if either body doesn't have a collider
					if (!bodyA->GetCollider() || !bodyB->GetCollider())
					{
						continue;
					}

					CollisionInfo info;
					if (CheckCollisions(*bodyA, *bodyB, info))
					{
						info.bodyA = bodyA;
						info.bodyB = bodyB;
						collisions.push_back(info);
					}
				}
			}
			
		}

		void PhysicsSystem::ResolveCollisions(std::vector<CollisionInfo>& collisions)
		{
			for (auto& collision: collisions)
			{
				auto bodyA = collision.bodyA;
				auto bodyB = collision.bodyB;

				//calculate relative velocity
				glm::vec3 relativeVelocity = bodyB->m_LinearVelocity - bodyA->m_LinearVelocity;

				//Calculate impulse magnitude
				float velocityAmongNormal = glm::dot(relativeVelocity, collision.normal);

				//Don't resolve if velocities are separating
				if (velocityAmongNormal >0)
				{
					continue;
				}

				//Calculate restitution (bounciness)
				float restitution = std::min(bodyA->m_Restitution, bodyB->m_Restitution);

				//Calculate impulse scalar
				float j = -(1.0f + restitution) * velocityAmongNormal;
				j /= bodyA->m_InverseMass + bodyB->m_InverseMass;

				//Apply impulse
				glm::vec3 impulse = collision.normal * j;

				if (!bodyA->IsKinematic())
				{
					bodyA->m_LinearVelocity -= impulse * bodyA->m_InverseMass;
				}

				if (!bodyB->IsKinematic())
				{
					bodyB->m_LinearVelocity += impulse * bodyB->m_InverseMass;
				}

				//Resolve penetration (positive correction)

				const float percent = 0.2f; //usually 20% do 80%
				const float slop = 0.01f; //small penetration allowed
				glm::vec3 correction = std::max(collision.penetrationDepth - slop, 0.0f) * percent * collision.normal / (bodyA->m_InverseMass + bodyB->m_InverseMass);

				if (!bodyA->IsKinematic())
				{
					bodyA->m_Position -= correction * bodyA->m_InverseMass;
				}

				if (!bodyB->IsKinematic())
				{
					bodyB->m_Position += correction * bodyB->m_InverseMass;
				}
			}
		}

		bool PhysicsSystem::CheckCollisions(const RigidBody& bodyA, const RigidBody& bodyB, CollisionInfo& info)
		{
			auto colliderA = bodyA.GetCollider();
			auto colliderB = bodyB.GetCollider();

			if (colliderA->GetType() == ColliderType::Sphere && colliderB->GetType() ==ColliderType::Sphere)
			{
				return SphereVSSphere(bodyA, bodyB, info);
			}
			else if (colliderA->GetType() == ColliderType::Box && colliderB->GetType() == ColliderType::Box)
			{
				return BoxVSBox(bodyA, bodyB, info);
			}
			/*else if (colliderA->GetType() == ColliderType::Sphere && colliderB->GetType() == ColliderType::Box) {
				return SphereVSBox(bodyA, bodyB, info);*/
			//} 
			/*else if (colliderA->GetType() == ColliderType::Box && colliderB->GetType() == ColliderType::Sphere)
			{
				bool result = SphereVSBox(bodyB, bodyA, info);
				if (result)
				{
					//Flip normal direction
					info.normal = -info.normal;
				}
				return result;
			}*/
			//unsupported collision type
			return false;
		}

		bool PhysicsSystem::SphereVSSphere(const RigidBody& bodyA, const RigidBody& bodyB, CollisionInfo& info)
		{
			auto sphereA = std::static_pointer_cast<SphereCollider>(bodyA.GetCollider());
			auto sphereB = std::static_pointer_cast<SphereCollider>(bodyB.GetCollider());

			glm::vec3 posA = bodyA.GetPosition() + sphereA->GetOffSet();
			glm::vec3 posB = bodyB.GetPosition() + sphereB->GetOffSet();

			float radiusA = sphereA->GetRadius();
			float radiusB = sphereB->GetRadius();

			glm::vec3 direction = posB - posA;
			float distance = glm::length(direction);
			float miniDistance = radiusA + radiusB;

			if (distance >= miniDistance)
			{
				return false;
			}

			//Normalize direction

			direction = distance > 0.0001f ? direction / distance : glm::vec3(0.0f, 1.0f, 0.0f);

			info.contactPoint = posA + direction * radiusA;
			info.normal = direction;
			info.penetrationDepth = miniDistance - distance;
			
			return true;
		}

		bool PhysicsSystem::BoxVSBox(const RigidBody& bodyA, const RigidBody& bodyB, CollisionInfo& info)
		{
			auto boxA = std::static_pointer_cast<BoxCollider>(bodyA.GetCollider());
			auto boxB = std::static_pointer_cast<BoxCollider>(bodyB.GetCollider());

			float leftA = bodyA.GetPosition().x - boxA->GetHalfExtents().x;
			float rightA = bodyA.GetPosition().x + boxA->GetHalfExtents().x;
			float topA = bodyA.GetPosition().y + boxA->GetHalfExtents().y;
			float bottomA = bodyA.GetPosition().y - boxA->GetHalfExtents().y;
			float frontA = bodyA.GetPosition().z + boxA->GetHalfExtents().z;
			float backA = bodyA.GetPosition().z - boxA->GetHalfExtents().z;

			float leftB = bodyB.GetPosition().x - boxB->GetHalfExtents().x;
			float rightB = bodyB.GetPosition().x + boxB->GetHalfExtents().x;
			float topB = bodyB.GetPosition().y + boxB->GetHalfExtents().y;
			float bottomB = bodyB.GetPosition().y - boxB->GetHalfExtents().y;
			float frontB = bodyB.GetPosition().z + boxB->GetHalfExtents().z;
			float backB = bodyB.GetPosition().z - boxB->GetHalfExtents().z;

			return (leftA < rightB && rightA > leftB && topA > bottomB && bottomA < topB && frontA > backB && backA < frontB);
			
		}
			
		bool PhysicsSystem::Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RaycastHit& hit)
		{
			glm::vec3 dir = glm::normalize(direction);
			bool found = false;
			float bestT = maxDistance;

			RaycastHit best{};
			best.distance = maxDistance;

			for (auto& body : m_RigidBodies)
			{
				auto col = body->GetCollider();
				if (!col) continue;

				float t = 0.0f;
				glm::vec3 n(0.0f);
				bool ok = false;

				if (col->GetType() == ColliderType::Sphere)
				{
					auto s = std::static_pointer_cast<SphereCollider>(col);
					glm::vec3 center = body->GetPosition() + s->GetOffSet();
					ok = RayVSSphere(origin, dir, maxDistance, center, s->GetRadius(), t, n);
				} else if (col->GetType() == ColliderType::Box)
				{
					auto b = std::static_pointer_cast<BoxCollider>(col);

					glm::vec3 center = body->GetPosition() + b->GetOffSet();

					glm::vec3 bMin = center - b->GetHalfExtents();
					glm::vec3 bMax = center + b->GetHalfExtents();

					ok = RayVsAABB(origin, dir, maxDistance, bMin, bMax, t, n);
				}

				if (ok && t < bestT)
				{
					bestT = t;
					best.point = origin + dir * t;
					best.normal = n;
					best.distance = t;
					best.body = body;
					found = true;
				}
			}
			if (found)
			{
				hit = best;
				return true;
			}
			return false;
		}
	}
//}