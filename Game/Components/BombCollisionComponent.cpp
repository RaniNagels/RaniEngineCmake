#include "BombCollisionComponent.h"
#include <Events/Event.h>
#include <GameObject.h>
#include "../Ids.h"
#include "BombComponent.h"
#include <stdexcept>

Game::BombCollisionComponent::BombCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& descriptor)
	: REC::CollisionComponent(owner, descriptor)
{
	m_pBombComponent = GetOwner()->GetComponent<BombComponent>();
	if (!m_pBombComponent)
		throw std::exception("must have a bomb component!");
}

void Game::BombCollisionComponent::OnCollision(const REC::GameObject* const other)
{
	if (other->Is(ObjectIds::Explosion))
	{
		m_pBombComponent->Detonate();
	}
}
