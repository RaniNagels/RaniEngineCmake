#include "PhysicsSystem.h"

#include <GameObject.h>
#include <Components/RigidBodyComponent.h>
#include <Components/TransformComponent.h>
#include <Components/CollisionComponent.h>

#include "CollisionSystem.h"

void REC::PhysicsSystem::Update(float)
{
	size_t index{};
	for (auto* physicsObject : m_PhysicsObjects)
	{
		if (physicsObject == nullptr) continue;
		auto* rigidBody = physicsObject->GetComponent<RigidBodyComponent>();
		auto* collisionBody = physicsObject->GetCollisionComponent();

		if (rigidBody == nullptr || collisionBody == nullptr)
		{
			assert(false && "Physics objects must have a rigid body and a collision component!");
			continue;
		}

		if (collisionBody->IsStatic()) continue;

		glm::vec2 velocity = rigidBody->GetVelocity();// *deltaTime;
		if (velocity == glm::vec2{ 0.f, 0.f }) continue;

		ResolveMovement(physicsObject, index, velocity);
		rigidBody->ResetVelocity();

		index++;
	}
}

// happens in the constructor of the rigid body component
void REC::PhysicsSystem::AddPhysicsObject(GameObject* physicsObject)
{
	if (physicsObject == nullptr) return;

	if (std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), physicsObject) == m_PhysicsObjects.end())
		m_PhysicsObjects.emplace_back(physicsObject);
}

void REC::PhysicsSystem::RemovePhysicsObject(GameObject* physicsObject)
{
	if (physicsObject == nullptr) return;

	if (const auto it = std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), physicsObject); it != m_PhysicsObjects.end())
		m_PhysicsObjects.erase(it);
}

void REC::PhysicsSystem::ResolveMovement(GameObject* obj, size_t index, const glm::vec2& movement)
{
	auto* transform = obj->GetTransform();

	// check for collisions at the new position
	// if there is a collision, move to the position right before the collision
	auto* collisionComp = obj->GetCollisionComponent();
	
	bool foundCollision = false;
	for (size_t i{index+1}; i < m_PhysicsObjects.size(); ++i)
	{
		// only check static vs non-static collisions, not dynamic vs dynamic
		auto* otherCollisionComp = m_PhysicsObjects[i]->GetCollisionComponent();
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
						if (m_pCollisionSystem->WillCollide(collisionComp, m_PhysicsObjects[i]->GetCollisionComponent(), testMovement, {}))
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
