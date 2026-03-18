#pragma once
#include <Components/Component.h>
#include <Event.h>
#include <memory>

namespace REC
{
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

		void SubscribeToEvents(IListener* listener);
		void UnSubscribeToEvents(IListener* listener);

	private:
		const float MAX_HEALTH;
		float m_CurrentHealth;

		std::unique_ptr<Event> m_HasZeroHealthEvent;
	};
}