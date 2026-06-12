#include "BalloomCollisionComponent.h"
#include <GameObject.h>
#include "../Ids.h"
#include <Components/LivesComponent.h>

Game::BalloomCollisionComponent::BalloomCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& descriptor)
	: REC::CollisionComponent(owner, descriptor)
{}

void Game::BalloomCollisionComponent::OnCollision(const REC::GameObject* const other)
{
	if (other->Is(ObjectIds::Explosion))
	{
		auto* livesComp = GetOwner()->GetComponent<REC::LivesComponent>();
		livesComp->LostLive();
	}
}
