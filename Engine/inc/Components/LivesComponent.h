#pragma once
#include <memory>

#include <Events/Event.h>
#include <Components/Component.h>
#include <Components/IListener.h>

namespace REC
{
	class HealthComponent;
	class LivesComponent final : public Component
	{
	public:
		explicit LivesComponent(GameObject* owner, int totalLives);
		virtual ~LivesComponent();

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) = delete;

		virtual void Update(float deltaT) override;

		void ResetLives() { m_CurrentAmountOfLives = MAX_LIVES; }
		bool HasLivesLeft() const { return m_CurrentAmountOfLives > 0; }

		int GetLives() const { return m_CurrentAmountOfLives; }
		void LostLive();

	private:
		const int MAX_LIVES;
		int m_CurrentAmountOfLives;
		bool m_LostLivesOnceInAFrame{ false };

		std::unique_ptr<Event> m_LostLiveEvent;
		std::unique_ptr<Event> m_HasDiedEvent;
		HealthComponent* m_pHealthComponent;
	};
}