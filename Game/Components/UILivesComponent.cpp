#include "UILivesComponent.h"
#include <Events/Event.h>
#include <sdbm_hash.h>

Game::UILivesComponent::UILivesComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
	SubscribeToEvent({ REC_EVENT_LOST_LIVE });
}

void Game::UILivesComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(REC_EVENT_LOST_LIVE))
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
