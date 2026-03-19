#include "UIScoreComponent.h"
#include <Event.h>
#include <sdbm_hash.h>

Game::UIScoreComponent::UIScoreComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
}

void Game::UIScoreComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(REC::make_sdbm_hash("HasPlacedBombEvent")))
	{
		AddToStatValue(30);
	}
}