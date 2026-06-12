#include "DoorCollisionComponent.h"
#include <GameObject.h>
#include "../Ids.h"
#include <Events/Event.h>

Game::DoorCollisionComponent::DoorCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& desc)
	: REC::CollisionComponent(owner, desc)
{
	m_pLevelChangeEvent = std::make_unique<REC::Event>(EventIds::DoorOpenEvent, REC::EventArgs{});
}

void Game::DoorCollisionComponent::OnCollision(const REC::GameObject* const other)
{
	if (other->Is(ObjectIds::Bomberman))
	{
		m_pLevelChangeEvent->Broadcast();
	}
}
