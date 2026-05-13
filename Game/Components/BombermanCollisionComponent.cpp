#include "BombermanCollisionComponent.h"

#include <GameObject.h>
#include <GeneralStructs.h>
#include <sdbm_hash.h>

#include "GridComponent.h"

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
	else if (other->Is(REC::make_sdbm_hash("Grid")))
	{
		//auto* transform = GetOwner()->GetTransform();
		//auto* grid = other->GetComponent<GridComponent>();
		//if (grid != nullptr)
		//{
		//	for (const auto& cell : grid->GetCells())
		//	{
		//		
		//	}
		//}
	}
}

void Game::BombermanCollisionComponent::OnCollisionEntry(const REC::GameObject* const)
{
}

void Game::BombermanCollisionComponent::OnCollisionExit(const REC::GameObject* const)
{
}
