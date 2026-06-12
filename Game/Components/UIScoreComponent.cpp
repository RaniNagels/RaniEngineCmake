#include "UIScoreComponent.h"
#include <Events/Event.h>
#include "../Ids.h"

Game::UIScoreComponent::UIScoreComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
	SubscribeToEvent({ EventIds::HasPlaceBombEvent, EventIds::DoorOpenEvent, REC::EventIds::DeathEvent });
}

void Game::UIScoreComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(EventIds::HasPlaceBombEvent))
	{
		auto* eventArgs = dynamic_cast<REC::GameObjectEventArgs*>(event->GetArgs());
		if (eventArgs != nullptr && eventArgs->sender == m_pConnectedPlayer)
		{
			AddToStatValue(30);
		}
	}
	if (event->IsEvent(EventIds::DoorOpenEvent)) // go to next level
	{
		AddToStatValue(100);
	}
	if (event->IsEvent(REC::EventIds::DeathEvent))
	{
		// add scores for killing enemies
		auto* eventArgs = dynamic_cast<REC::GameObjectEventArgs*>(event->GetArgs());
		auto* gameObject = eventArgs->sender;
		if (gameObject->Is(ObjectIds::Balloom))
		{
			AddToStatValue(100);
		}
		else if (gameObject->Is(ObjectIds::Oneal))
		{
			AddToStatValue(200);
		}
		else if (gameObject->Is(ObjectIds::Doll))
		{
			AddToStatValue(400);
		}
		else if (gameObject->Is(ObjectIds::Minvo))
		{
			AddToStatValue(800);
		}
	}
}

void Game::UIScoreComponent::Destroy()
{
	UnsubscribeFromEvent({ EventIds::HasPlaceBombEvent, EventIds::DoorOpenEvent, REC::EventIds::DeathEvent });
	LabeledStatComponent::Destroy();
}