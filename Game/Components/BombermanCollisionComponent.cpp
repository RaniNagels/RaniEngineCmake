#include "BombermanCollisionComponent.h"
#include <GameObject.h>
#include <sdbm_hash.h>
#include <iostream>

Game::BombermanCollisionComponent::BombermanCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& descriptor)
	: REC::CollisionComponent(owner, descriptor)
{
}

void Game::BombermanCollisionComponent::OnCollision(const REC::GameObject* const other)
{
	if (other->Is(REC::make_sdbm_hash("Balloom")))
	{
		std::cout << "Bomberman collided with a Balloom!" << std::endl;
	}
}

void Game::BombermanCollisionComponent::OnCollisionEntry(const REC::GameObject* const)
{
}

void Game::BombermanCollisionComponent::OnCollisionExit(const REC::GameObject* const)
{
}
