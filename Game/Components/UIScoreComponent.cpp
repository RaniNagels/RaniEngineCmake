#include "UIScoreComponent.h"
#include <Events/Event.h>
#include <sdbm_hash.h>

Game::UIScoreComponent::UIScoreComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
	SubscribeToEvent({ REC::make_sdbm_hash("HasPlacedBombEvent") });
}

void Game::UIScoreComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(REC::make_sdbm_hash("HasPlacedBombEvent")))
	{
		auto* eventArgs = dynamic_cast<REC::GameObjectEventArgs*>(event->GetArgs());
		if (eventArgs != nullptr && eventArgs->sender == m_pConnectedPlayer)
		{
			AddToStatValue(30);
		}
	}
}