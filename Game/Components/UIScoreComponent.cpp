#include "UIScoreComponent.h"
#include <Events/Event.h>
#include "../Ids.h"

Game::UIScoreComponent::UIScoreComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
	SubscribeToEvent({ Game::EventIds::HasPlaceBombEvent });
}

void Game::UIScoreComponent::Notify(REC::Event* event)
{
	//TODO
	if (event->IsEvent(Game::EventIds::HasPlaceBombEvent))
	{
		auto* eventArgs = dynamic_cast<REC::GameObjectEventArgs*>(event->GetArgs());
		if (eventArgs != nullptr && eventArgs->sender == m_pConnectedPlayer)
		{
			AddToStatValue(30);
		}
	}
	if (event->IsEvent(Game::EventIds::DoorOpenEvent)) // go to next level
	{
		AddToStatValue(100);
	}
}

void Game::UIScoreComponent::Destroy()
{
	UnsubscribeFromEvent({ Game::EventIds::HasPlaceBombEvent });
	LabeledStatComponent::Destroy();
}