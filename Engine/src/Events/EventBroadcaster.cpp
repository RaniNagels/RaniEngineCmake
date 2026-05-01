#include <Events/EventBroadcaster.h>

#include <Events/EventSystem.h>
#include <Components/IListener.h>

#include "CollisionSystem.h"

void REC::EventBroadcaster::Broadcast(Event* event)
{
	if (event != nullptr)
		m_pEventSystem->BroadcastEvent(event);
}

void REC::EventBroadcaster::Broadcast(const CollisionEvent& event)
{
	m_pEventSystem->BroadcastCollisionEvent(event);
}

void REC::EventBroadcaster::Subscribe(IListener* subscriber, std::initializer_list<EventId> events)
{
	if (m_pEventSystem)
		m_pEventSystem->Subscribe(subscriber, events);
}

void REC::EventBroadcaster::Unsubscribe(IListener* subscriber, std::initializer_list<EventId> events)
{
	if (m_pEventSystem)
		m_pEventSystem->Unsubscribe(subscriber, events);
}

void REC::EventBroadcaster::Subscribe(CollisionComponent* subscriber)
{
	if (m_pCollisionSystem)
		m_pCollisionSystem->Subscribe(subscriber);
}

void REC::EventBroadcaster::Unsubscribe(CollisionComponent* subscriber)
{
	if (m_pCollisionSystem)
		m_pCollisionSystem->Unsubscribe(subscriber);
}
