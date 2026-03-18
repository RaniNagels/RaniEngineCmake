#include "UIScoreComponent.h"
#include <Event.h>
#include <sdbm_hash.h>

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

Game::UIScoreComponent::UIScoreComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor)
	: LabeledStatComponent(owner, descriptor)
{
}

void Game::UIScoreComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(REC::make_sdbm_hash("ValueChangedEvent")))
	{
		auto e = static_cast<REC::ValueChangedEvent*>(event);
		AddToStatValue(int(e->GetDelta()));

#if USE_STEAMWORKS
		// should not keep track of the score in the UI component
		// ideally, the score should be in a separate component
		if (GetStatValue() >= 500)
		{
			if (SteamUserStats())
			{
				bool achieved = false;
				SteamUserStats()->GetAchievement("ACH_WIN_ONE_GAME", &achieved);
				if (!achieved) 
				{
					SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
					SteamUserStats()->StoreStats();
				}
			}
		}
#endif
	}
}