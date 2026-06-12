#include "DoorCollisionComponent.h"
#include <GameObject.h>
#include "../Ids.h"
#include <Events/Event.h>
#include <Scene.h>

Game::DoorCollisionComponent::DoorCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& desc, REC::Scene* scene)
	: REC::CollisionComponent(owner, desc)
	, m_pScene(scene)
{
	m_pLevelChangeEvent = std::make_unique<REC::Event>(EventIds::DoorOpenEvent, REC::EventArgs{});
}

void Game::DoorCollisionComponent::OnCollision(const REC::GameObject* const other)
{
	auto enemies = m_pScene->GetGameObjects(ObjectIds::Balloom);
	if (!enemies.empty())
		return;

	enemies = m_pScene->GetGameObjects(ObjectIds::Doll);
	if (!enemies.empty())
		return;

	enemies = m_pScene->GetGameObjects(ObjectIds::Oneal);
	if (!enemies.empty())
		return;

	enemies = m_pScene->GetGameObjects(ObjectIds::Minvo);
	if (!enemies.empty())
		return;

	if (other->Is(ObjectIds::Bomberman))
	{
		m_pLevelChangeEvent->Broadcast();
	}
}
