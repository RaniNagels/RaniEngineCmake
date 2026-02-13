#include "../inc/TransformComponent.h"

REC::TransformComponent::TransformComponent(const glm::vec3& position)
	: m_Position{position}
{
}

REC::TransformComponent::TransformComponent(float x, float y, float z)
	: m_Position{x, y, z}
{
}

void REC::TransformComponent::Update(float)
{
}

void REC::TransformComponent::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void REC::TransformComponent::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}
