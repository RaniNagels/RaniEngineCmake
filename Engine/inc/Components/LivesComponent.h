#pragma once
#include <Components/Component.h>
#include <Event.h>
#include <memory>
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

		void SubscribeToEvents(IListener* listener);
		void UnSubscribeToEvents(IListener* listener);

	private:
		const int MAX_LIVES;
		int m_CurrentAmountOfLives;

		std::unique_ptr<Event> m_LostLiveEvent;
		HealthComponent* m_pHealthComponent;
	};
}