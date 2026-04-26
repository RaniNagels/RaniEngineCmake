#pragma once
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <Events/Event.h>

namespace REC
{
	using EventId = unsigned int;

	class Event;
	class IListener;

	class EventSystem final
	{
	public:
		explicit EventSystem() = default;
		~EventSystem() = default;

		EventSystem(const EventSystem& other) = delete;
		EventSystem(EventSystem&& other) noexcept = delete;
		EventSystem& operator=(const EventSystem& other) = delete;
		EventSystem& operator=(EventSystem&& other) noexcept = delete;

		void ProcessEvents();

		void BroadcastEvent(Event* event);
		void BroadcastCollisionEvent(const CollisionEvent& event);

		void Subscribe(IListener* subscriber, std::initializer_list<EventId> events);
		void Unsubscribe(IListener* subscriber, std::initializer_list<EventId> events = {});

	private:
		std::queue<Event*> m_EventQueue{}; // non-owning
		std::queue<CollisionEvent> m_CollisionEventQueue{}; // owning (temporary)
		std::unordered_map<EventId, std::vector<IListener*>> m_EventSubscribers{};

		void ProcessCollisionEvents();
		void ProcessNormalEvents();
	};
}