#include "UICurrentLevelComponent.h"
#include <Events/Event.h>
#include <GameObject.h>
#include "../Ids.h"

Game::UICurrentLevelComponent::UICurrentLevelComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
	SubscribeToEvent({ EventIds::LevelChangeEvent });
}

void Game::UICurrentLevelComponent::Notify(REC::Event * event)
{
	if (event->IsEvent(EventIds::LevelChangeEvent))
	{
		AddToStatValue(1);
	}
}

void Game::UICurrentLevelComponent::Destroy()
{
	UnsubscribeFromEvent({ EventIds::LevelChangeEvent });
	LabeledStatComponent::Destroy();
}
