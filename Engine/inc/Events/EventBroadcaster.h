#pragma once
#include <Events/EventSystem.h>

namespace REC
{
	class Event;
	class EventSystem;

	class EventBroadcaster final
	{
	public:
		EventBroadcaster() = default;
		~EventBroadcaster() = default;

		EventBroadcaster(const EventBroadcaster& other) = delete;
		EventBroadcaster(EventBroadcaster&& other) = delete;
		EventBroadcaster& operator=(const EventBroadcaster& other) = delete;
		EventBroadcaster& operator=(EventBroadcaster&& other) = delete;

		static void SetEventSystem(EventSystem* eventSystem) { m_pEventSystem = eventSystem; }

		void Broadcast(Event* event)
		{
			if (event != nullptr)
				m_pEventSystem->BroadcastEvent(event);
		}

	private:
		inline static EventSystem* m_pEventSystem = nullptr;
	};
}