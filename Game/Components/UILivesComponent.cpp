#include "UILivesComponent.h"
#include <Events/Event.h>

Game::UILivesComponent::UILivesComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
	SubscribeToEvent({ REC::EventIds::LostLive });
}

void Game::UILivesComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(REC::EventIds::LostLive))
	{
		auto* eventArgs = dynamic_cast<REC::GameObjectEventArgs*>(event->GetArgs());
		if (eventArgs != nullptr && eventArgs->sender == m_pConnectedPlayer)
		{
			AddToStatValue(-1);

			// when owner lost all lives
			// UnsubscribeFromEvent({ REC::make_sdbm_hash("LostLiveEvent") });
		}
	}
}

void Game::UILivesComponent::Destroy()
{
	UnsubscribeFromEvent({ REC::EventIds::LostLive });
	LabeledStatComponent::Destroy();
}
