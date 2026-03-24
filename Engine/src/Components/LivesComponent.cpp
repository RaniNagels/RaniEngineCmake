#include <Components/LivesComponent.h>
#include <Components/HealthComponent.h>
#include <GameObject.h>

REC::LivesComponent::LivesComponent(GameObject* owner, int totalLives)
	: Component(owner)
	, MAX_LIVES{totalLives}
	, m_CurrentAmountOfLives{totalLives}
	, m_LostLiveEvent{std::make_unique<Event>(GetOwner(), make_sdbm_hash("LostLiveEvent"))}
	, m_pHealthComponent{GetOwner()->GetComponent<HealthComponent>()}
{
}

REC::LivesComponent::~LivesComponent() = default;

void REC::LivesComponent::Update(float) {}

void REC::LivesComponent::Notify(Event* event)
{
	if (event->IsEvent(make_sdbm_hash("HasZeroHealthEvent")) && event->IsSender(GetOwner()))
	{
		m_pHealthComponent->ResetHealth();
		if (m_CurrentAmountOfLives > 0)
		{
			--m_CurrentAmountOfLives;
			m_LostLiveEvent->Broadcast();
		}
	}
}
