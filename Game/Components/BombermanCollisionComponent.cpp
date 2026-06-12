#include "BombermanCollisionComponent.h"

#include <GameObject.h>
#include <GeneralStructs.h>
#include <sdbm_hash.h>

#include "GridComponent.h"
#include "../Ids.h"

#include <iostream>
#include <Components/LivesComponent.h>

Game::BombermanCollisionComponent::BombermanCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& descriptor)
	: REC::CollisionComponent(owner, descriptor)
{
}

void Game::BombermanCollisionComponent::OnCollision(const REC::GameObject* const other)
{
	if (other->Is(Game::ObjectIds::Balloom) || other->Is(Game::ObjectIds::Explosion))
	{
		auto* livesComp = GetOwner()->GetComponent<REC::LivesComponent>();
		livesComp->LostLive();
	}
	else if (other->Is(Game::ObjectIds::PickUp))
	{
		std::cout << "Bomberman collided with a PickUp!" << std::endl;
	}
}

void Game::BombermanCollisionComponent::OnCollisionEntry(const REC::GameObject* const)
{
}

void Game::BombermanCollisionComponent::OnCollisionExit(const REC::GameObject* const)
{
}
