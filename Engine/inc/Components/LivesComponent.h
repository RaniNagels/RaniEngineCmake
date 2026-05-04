#pragma once
#include <memory>

#include <Events/Event.h>
#include <Components/Component.h>
#include <Components/IListener.h>

namespace REC
{
	class HealthComponent;
	class LivesComponent final : public Component, public IListener
	{
	public:
		explicit LivesComponent(GameObject* owner, int totalLives);
		virtual ~LivesComponent();

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		virtual void Notify(Event* event) override;

		void ResetLives() { m_CurrentAmountOfLives = MAX_LIVES; }
		bool HasLivesLeft() const { return m_CurrentAmountOfLives > 0; }

	private:
		const int MAX_LIVES;
		int m_CurrentAmountOfLives;

		std::unique_ptr<Event> m_LostLiveEvent;
		HealthComponent* m_pHealthComponent;
	};
}