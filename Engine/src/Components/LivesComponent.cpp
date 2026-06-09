#include <Components/LivesComponent.h>

#include <Components/HealthComponent.h>
#include <GameObject.h>

REC::LivesComponent::LivesComponent(GameObject* owner, int totalLives)
	: Component(owner)
	, MAX_LIVES{totalLives}
	, m_CurrentAmountOfLives{totalLives}
	, m_pHealthComponent{GetOwner()->GetComponent<HealthComponent>()}
{
	GameObjectEventArgs args{};
	args.sender = GetOwner();
	m_LostLiveEvent = std::make_unique<Event>(REC::EventIds::LostLive, args);
	m_HasDiedEvent = std::make_unique<Event>(REC::EventIds::DeathEvent, args);
}

REC::LivesComponent::~LivesComponent() = default;

void REC::LivesComponent::Update(float) {}

void REC::LivesComponent::LostLive()
{
	if (m_pHealthComponent) m_pHealthComponent->ResetHealth();
	if (m_CurrentAmountOfLives > 0)
	{
		--m_CurrentAmountOfLives;
		m_LostLiveEvent->Broadcast();

		if (m_CurrentAmountOfLives == 0)
			m_HasDiedEvent->Broadcast();
	}
}
