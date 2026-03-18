#include "UILivesComponent.h"
#include <Event.h>
#include <sdbm_hash.h>

Game::UILivesComponent::UILivesComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
}

void Game::UILivesComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(REC::make_sdbm_hash("LostLiveEvent")))
	{
		AddToStatValue(-1);
	}
}
