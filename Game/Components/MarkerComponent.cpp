#include "MarkerComponent.h"
#include <GameObject.h>
#include <Components/TransformComponent.h>

Game::MarkerComponent::MarkerComponent(REC::GameObject* owner, const MarkerDescriptor& desc)
	: REC::Component(owner)
	, m_Descriptor(desc)
{
	m_CurrentPosition = m_Descriptor.positions.empty() ? glm::vec2{ 0.f, 0.f } : m_Descriptor.positions[0];
}

void Game::MarkerComponent::Update(float)
{}

void Game::MarkerComponent::MoveToNextPosition(bool moveUp)
{
	glm::vec2 nextPos2D = GetNextPosition(moveUp);
	glm::vec3 nextPosition = { nextPos2D, 0 };
	GetOwner()->GetTransform()->SetLocalPosition(nextPosition);
	m_CurrentPosition = nextPos2D;
}

uint8_t Game::MarkerComponent::GetCurrentIndex() const
{
	for (size_t i{}; i < m_Descriptor.positions.size(); ++i)
	{
		if (m_Descriptor.positions[i] == m_CurrentPosition)
			return static_cast<uint8_t>(i);
	}

	return uint8_t(-1);
}

glm::vec2 Game::MarkerComponent::GetNextPosition(bool moveUp) const
{
	for (size_t i{}; i < m_Descriptor.positions.size(); ++i)
	{
		if (m_Descriptor.positions[i] == m_CurrentPosition)
		{
			if (moveUp)
			{
				size_t nextIndex = (i == 0) ? m_Descriptor.positions.size() - 1 : i - 1;
				return m_Descriptor.positions[nextIndex];
			}
			else
			{
				size_t nextIndex = (i + 1) % m_Descriptor.positions.size();
				return m_Descriptor.positions[nextIndex];
			}
		}
	}
	
	return m_CurrentPosition;
}