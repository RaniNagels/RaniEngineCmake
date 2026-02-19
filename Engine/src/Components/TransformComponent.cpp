#include "../../inc/Components/TransformComponent.h"
#include "../../inc/GameObject.h"

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

void REC::TransformComponent::Update(float) { }

glm::vec3 REC::TransformComponent::GetWorldPosition()
{
	if (m_NeedsUpdate)
	{
		auto parent = GetOwner()->GetParent();
		if (parent != nullptr)
		{
			m_WorldPosition = parent->GetTransform()->GetWorldPosition() + GetLocalPosition();
		}
		else
			m_WorldPosition = GetLocalPosition();
		
		m_NeedsUpdate = false;
	}

	return m_WorldPosition;
}

void REC::TransformComponent::AddToLocalPosition(float x, float y, float z)
{
	AddToLocalPosition(glm::vec3{ x, y, z });
}

void REC::TransformComponent::AddToLocalPosition(const glm::vec3& position)
{
	m_Position += position;
	RequiresUpdate();
}

void REC::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	SetLocalPosition(glm::vec3{ x, y, z });
}

void REC::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_Position = position;
	RequiresUpdate();
}

void REC::TransformComponent::RequiresUpdate()
{
	m_NeedsUpdate = true;
	for (auto* child : GetOwner()->GetChildren())
	{
		child->GetTransform()->RequiresUpdate();
	}
}
