#pragma once
#include <initializer_list>

namespace REC
{
	class Event;
	class CollisionEvent;
	class EventSystem;
	class CollisionSystem;
	class CollisionComponent;

	class IListener;
	class ICollisionListener;

	using EventId = unsigned int;

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
		static void SetCollisionSystem(CollisionSystem* collisionSystem) { m_pCollisionSystem = collisionSystem; }

		static void Broadcast(Event* event);
		static void Broadcast(const CollisionEvent& event);

		static void Subscribe(IListener* subscriber, std::initializer_list<EventId> events);
		static void Unsubscribe(IListener* subscriber, std::initializer_list<EventId> events);

		static void Subscribe(CollisionComponent* subscriber);
		static void Unsubscribe(CollisionComponent* subscriber);

	private:
		inline static EventSystem* m_pEventSystem = nullptr;
		inline static CollisionSystem* m_pCollisionSystem = nullptr;
	};
}