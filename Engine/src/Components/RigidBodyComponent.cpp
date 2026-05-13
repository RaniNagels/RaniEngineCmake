#include <Components/RigidBodyComponent.h>

#include "../PhysicsSystem.h"
#include <GameObject.h>

#include <stdexcept>

REC::PhysicsSystem* REC::RigidBodyComponent::s_PhysicsSystem = nullptr;

REC::RigidBodyComponent::RigidBodyComponent(GameObject* owner)
	: Component(owner)
{
	s_PhysicsSystem->AddPhysicsObject(GetOwner());
	m_IsStatic = GetOwner()->GetCollisionComponent()->IsStatic();
}

REC::RigidBodyComponent::~RigidBodyComponent() = default;

void REC::RigidBodyComponent::Destroy()
{
	Component::Destroy();
	if (s_PhysicsSystem)
		s_PhysicsSystem->RemovePhysicsObject(GetOwner());
}

void REC::RigidBodyComponent::SetVelocity(const glm::vec2& velocity)
{
	if (m_IsStatic) throw std::logic_error("Cannot set velocity on a static rigid body!");

	m_Velocity = velocity;
}

void REC::RigidBodyComponent::AddToVelocity(const glm::vec2& velocity)
{
	if (m_IsStatic) throw std::logic_error("Cannot set velocity on a static rigid body!");

	m_Velocity += velocity;
}