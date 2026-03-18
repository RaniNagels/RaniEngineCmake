#include <Components/LivesComponent.h>
#include <Components/HealthComponent.h>
#include <GameObject.h>

REC::LivesComponent::LivesComponent(GameObject* owner, int totalLives)
	: Component(owner)
	, MAX_LIVES{totalLives}
	, m_CurrentAmountOfLives{totalLives}
	, m_LostLiveEvent{std::make_unique<Event>(make_sdbm_hash("LostLiveEvent"))}
	, m_pHealthComponent{GetOwner()->GetComponent<HealthComponent>()}
{
}

REC::LivesComponent::~LivesComponent() = default;

void REC::LivesComponent::Update(float) {}

void REC::LivesComponent::Notify(Event* event)
{
	if (event->IsEvent(make_sdbm_hash("HasZeroHealthEvent")))
	{
		m_pHealthComponent->ResetHealth();
		if (m_CurrentAmountOfLives > 0)
		{
			--m_CurrentAmountOfLives;
			m_LostLiveEvent->NotifyListeners();
		}
	}
}

void REC::LivesComponent::SubscribeToEvents(IListener* listener)
{
	m_LostLiveEvent->Subscribe(listener);
}

void REC::LivesComponent::UnSubscribeToEvents(IListener* listener)
{
	m_LostLiveEvent->Unsubscribe(listener);
}