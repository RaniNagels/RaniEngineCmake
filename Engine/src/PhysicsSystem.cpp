#include "PhysicsSystem.h"

#include <GameObject.h>
#include <Components/RigidBodyComponent.h>
#include <Components/TransformComponent.h>
#include <Components/CollisionComponent.h>

#include "CollisionSystem.h"

void REC::PhysicsSystem::Update(float)
{
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
		ResolveMovement(physicsObject, velocity);
		rigidBody->ResetVelocity();
	}
}

void REC::PhysicsSystem::AddPhysicsObject(GameObject* physicsObject)
{
	if (physicsObject == nullptr) return;

	//auto* rigidBody = physicsObject->GetComponent<RigidBodyComponent>();
	//if (rigidBody == nullptr)
	//{
	//	assert(false && "Cannot add a physics object without a rigid body component to the physics system!");
	//	return;
	//}

	if (std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), physicsObject) == m_PhysicsObjects.end())
		m_PhysicsObjects.emplace_back(physicsObject);
}

void REC::PhysicsSystem::RemovePhysicsObject(GameObject* physicsObject)
{
	if (physicsObject == nullptr) return;

	if (const auto it = std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), physicsObject); it != m_PhysicsObjects.end())
		m_PhysicsObjects.erase(it);
}

void REC::PhysicsSystem::ResolveMovement(GameObject* obj, const glm::vec2& movement)
{
	auto* transform = obj->GetTransform();
	if (movement == glm::vec2{ 0.f, 0.f }) return;

	// check for collisions at the new position
	// if there is a collision, move to the position right before the collision
	auto* collisionComp = obj->GetCollisionComponent();
	
	if (m_pCollisionSystem->WillCollideWithStatic(collisionComp, movement))
	{
		// collision: move to the position right before the collision
		constexpr float stepSize = 1.f;
		glm::vec2 direction = glm::normalize(movement);
		float movementLength = glm::length(movement);

		glm::vec2 validMovement{};

		// move in small steps until we find the position right before the collision
		for (float movedDistance = stepSize; movedDistance < movementLength; movedDistance += stepSize)
		{
			glm::vec2 testMovement = direction * movedDistance;
			if (m_pCollisionSystem->WillCollideWithStatic(collisionComp, testMovement))
				break;

			validMovement += testMovement;
		}
		transform->AddToLocalPosition(validMovement.x, validMovement.y);
	}
	else // no collision
		transform->AddToLocalPosition(movement.x, movement.y);
}
