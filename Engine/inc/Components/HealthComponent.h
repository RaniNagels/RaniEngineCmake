#pragma once
#include <memory>

#include <Components/Component.h>
#include <Events/Event.h>

namespace REC
{
	// not used
	class IListener;
	class HealthComponent final : public Component
	{
	public:
		explicit HealthComponent(GameObject* owner, float maxHealth, float currentHealth);
		virtual ~HealthComponent();

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		virtual void Update(float deltaT) override;

		void ChangeHealth(float delta);
		void ResetHealth();

	private:
		const float MAX_HEALTH;
		float m_CurrentHealth;

		std::unique_ptr<Event> m_HasZeroHealthEvent;
	};
}