#include <Components/LivesComponent.h>

#include <Components/HealthComponent.h>
#include <GameObject.h>
#include <sdbm_hash.h>

REC::LivesComponent::LivesComponent(GameObject* owner, int totalLives)
	: Component(owner)
	, MAX_LIVES{totalLives}
	, m_CurrentAmountOfLives{totalLives}
	, m_pHealthComponent{GetOwner()->GetComponent<HealthComponent>()}
{
	GameObjectEventArgs args{};
	args.sender = GetOwner();
	m_LostLiveEvent = std::make_unique<Event>(make_sdbm_hash("LostLiveEvent"), args);

	SubscribeToEvent({ REC_EVENT_HAS_ZERO_HEALTH });
}

REC::LivesComponent::~LivesComponent() = default;

void REC::LivesComponent::Update(float) {}

void REC::LivesComponent::Notify(Event* event)
{
	if (event->IsEvent(make_sdbm_hash("HasZeroHealthEvent")))
	{
		auto* eventArgs = dynamic_cast<GameObjectEventArgs*>(event->GetArgs());
		if (eventArgs != nullptr && eventArgs->sender == GetOwner())
		{
			m_pHealthComponent->ResetHealth();
			if (m_CurrentAmountOfLives > 0)
			{
				--m_CurrentAmountOfLives;
				m_LostLiveEvent->Broadcast();
			}
		}
	}
}
