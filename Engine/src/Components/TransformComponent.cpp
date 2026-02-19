#include "../../inc/Components/TransformComponent.h"

REC::TransformComponent::TransformComponent(GameObject* owner)
	: Component(owner)
{ }

REC::TransformComponent::TransformComponent(GameObject* owner, const glm::vec3& position)
	: Component(owner)
	, m_Position{position}
{
}

REC::TransformComponent::TransformComponent(GameObject* owner, float x, float y, float z)
	: TransformComponent(owner, glm::vec3{x, y, z})
{ }

void REC::TransformComponent::Update(float)
{
}

void REC::TransformComponent::SetPosition(float x, float y, float z)
{
	SetPosition(glm::vec3{ x, y, z });
}

void REC::TransformComponent::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}
