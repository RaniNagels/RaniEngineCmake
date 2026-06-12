#include "BombermanCollisionComponent.h"

#include <GameObject.h>
#include <GeneralStructs.h>
#include <sdbm_hash.h>

#include "GridComponent.h"
#include "../Ids.h"

#include <iostream>
#include <Components/LivesComponent.h>
#include "BombermanComponent.h"
#include "PickUpComponent.h"

Game::BombermanCollisionComponent::BombermanCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& descriptor)
	: REC::CollisionComponent(owner, descriptor)
{
}

void Game::BombermanCollisionComponent::OnCollision(const REC::GameObject* const other)
{
	if (other->Is(ObjectIds::Balloom) || other->Is(ObjectIds::Explosion)
		|| other->Is(ObjectIds::Doll) || other->Is(ObjectIds::Minvo)
		|| other->Is(ObjectIds::Oneal))
	{
		auto* livesComp = GetOwner()->GetComponent<REC::LivesComponent>();
		livesComp->LostLive();
	}
	else if (other->Is(Game::ObjectIds::PickUp))
	{
		std::cout << "Bomberman collided with a PickUp!" << std::endl;
		auto* bomComp = GetOwner()->GetComponent<BombermanComponent>();
		auto* pickup = other->GetComponent<PickUpComponent>();
		if (pickup->IsType(PickUpComponent::PickUpType::ExtraBomb))
		{
			bomComp->SetPickupExtraBomb(true);
		}
		else if (pickup->IsType(PickUpComponent::PickUpType::Detonator))
		{
			bomComp->SetPickupDetonator(true);
		}
		else if (pickup->IsType(PickUpComponent::PickUpType::Flames))
		{
			bomComp->SetPickupFlames(true);
		}
		pickup->GetOwner()->Destroy();
	}
}

void Game::BombermanCollisionComponent::OnCollisionEntry(const REC::GameObject* const)
{
}

void Game::BombermanCollisionComponent::OnCollisionExit(const REC::GameObject* const)
{
}
