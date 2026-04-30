#include <Components/HealthComponent.h>
#include <sdbm_hash.h>

REC::HealthComponent::HealthComponent(GameObject* owner, float maxHealth, float currentHealth)
	: Component(owner)
	, MAX_HEALTH{maxHealth}
	, m_CurrentHealth{currentHealth}
{
	GameObjectEventArgs args{};
	args.sender = GetOwner();
	m_HasZeroHealthEvent = std::make_unique<Event>(REC::make_sdbm_hash("HasZeroHealthEvent"), args);
}

REC::HealthComponent::~HealthComponent() = default;

void REC::HealthComponent::Update(float) {}

void REC::HealthComponent::ChangeHealth(float delta)
{
	if (m_CurrentHealth > 0)
	{
		m_CurrentHealth += delta;
		if (m_CurrentHealth > MAX_HEALTH)
			m_CurrentHealth = MAX_HEALTH;
		else if (m_CurrentHealth <= 0)
			m_HasZeroHealthEvent->Broadcast();
	}
}

void REC::HealthComponent::ResetHealth()
{
	m_CurrentHealth = MAX_HEALTH;
}
