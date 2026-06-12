#include "EnemyMovementComponent.h"
#include <GameObject.h>
#include <Scene.h>
#include "GridComponent.h"
#include <Events/Event.h>

Game::EnemyMovementComponent::EnemyMovementComponent(REC::GameObject* owner, GridComponent* grid, REC::Scene* scene, float speed)
	: REC::Component(owner)
	, m_Speed(speed)
	, m_pGrid(grid)
	, m_pScene(scene)
{
	SubscribeToEvent({ REC::EventIds::DeathEvent });
}

Game::EnemyMovementComponent::~EnemyMovementComponent()
{
	UnsubscribeFromEvent({ REC::EventIds::DeathEvent });
}

void Game::EnemyMovementComponent::Update(float)
{}

void Game::EnemyMovementComponent::Notify(REC::Event * event)
{
	if (event->IsEvent(REC::EventIds::DeathEvent))
	{
		auto args = static_cast<REC::GameObjectEventArgs*>(event->GetArgs());
		if (args->sender == GetOwner())
		{
			GetOwner()->Destroy();
		}
	}
}
