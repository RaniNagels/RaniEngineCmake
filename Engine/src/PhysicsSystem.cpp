#include "PhysicsSystem.h"

#include <GameObject.h>
#include <Components/RigidBodyComponent.h>
#include <Components/TransformComponent.h>
#include <Components/CollisionComponent.h>

#include "CollisionSystem.h"

void REC::PhysicsSystem::Update(float)
{
	for (auto* physicsObject : m_RigidBodies)
	{
		if (physicsObject == nullptr) continue;

		glm::vec2 velocity = physicsObject->GetVelocity();// *deltaTime;
		if (velocity == glm::vec2{ 0.f, 0.f }) continue;

		ResolveMovement(physicsObject->GetOwner(), velocity);
		physicsObject->ResetVelocity();
	}
}

// happens in the constructor of the rigid body component
void REC::PhysicsSystem::AddPhysicsObject(RigidBodyComponent* physicsObject)
{
	if (physicsObject == nullptr) return;
	if (physicsObject->GetOwner()->GetCollisionComponent()->IsStatic())
	{
		 if (std::find(m_StaticRigidBodies.begin(), m_StaticRigidBodies.end(), physicsObject) == m_StaticRigidBodies.end())
			 m_StaticRigidBodies.emplace_back(physicsObject);
	}
	else
	{
		if (std::find(m_RigidBodies.begin(), m_RigidBodies.end(), physicsObject) == m_RigidBodies.end())
			m_RigidBodies.emplace_back(physicsObject);
	}
}

void REC::PhysicsSystem::RemovePhysicsObject(RigidBodyComponent* physicsObject)
{
	if (physicsObject == nullptr) return;
	if (physicsObject->GetOwner()->GetCollisionComponent()->IsStatic())
	{
		if (!m_StaticRigidBodies.empty())
		{
			if (const auto it = std::find(m_StaticRigidBodies.begin(), m_StaticRigidBodies.end(), physicsObject); it != m_StaticRigidBodies.end())
				m_StaticRigidBodies.erase(it);
		}
	}
	else
	{
		if (!m_RigidBodies.empty())
		{
			if (const auto it = std::find(m_RigidBodies.begin(), m_RigidBodies.end(), physicsObject); it != m_RigidBodies.end())
				m_RigidBodies.erase(it);
		}
	}
}

void REC::PhysicsSystem::ResolveMovement(GameObject* obj, const glm::vec2& movement)
{
	auto* transform = obj->GetTransform();

	// check for collisions at the new position
	// if there is a collision, move to the position right before the collision
	auto* collisionComp = obj->GetCollisionComponent();
	
	bool foundCollision = false;
	for (size_t i{}; i < m_StaticRigidBodies.size(); ++i)
	{
		// only check static vs non-static collisions, not dynamic vs dynamic
		auto* otherCollisionComp = m_StaticRigidBodies[i]->GetOwner()->GetCollisionComponent();
		if (otherCollisionComp->IsStatic())
		{
			if (m_pCollisionSystem->WillCollide(collisionComp, otherCollisionComp, movement, {}))
			{
				// collision: move to the position right before the collision
				foundCollision = true;
				if (movement.x != 0.f)
				{
					constexpr float stepSize = 1.f;
					glm::vec2 direction = glm::normalize(glm::vec2{ movement.x, 0.f });
					float movementLength = std::abs(movement.x);
					glm::vec2 validMovement{};

					// move in small steps until we find the position right before the collision
					for (float movedDistance = stepSize; movedDistance < movementLength; movedDistance += stepSize)
					{
						glm::vec2 testMovement = direction * movedDistance;
						if (m_pCollisionSystem->WillCollide(collisionComp, otherCollisionComp, testMovement, {}))
							break;
						validMovement += testMovement;
					}
					transform->AddToLocalPosition(validMovement.x, 0.f);
				}
			}
		}
	}

	if (!foundCollision)
		transform->AddToLocalPosition(movement.x, movement.y);
}
