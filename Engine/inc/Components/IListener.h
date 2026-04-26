#pragma once
#include <Events/EventBroadcaster.h>

namespace REC
{
	class Event;
	using EventId = unsigned int;
	class GameObject;

	// component class can inherit from it!!
	// TODO: no longer a pure interface
	class IListener // aka Observer
	{
	public:
		virtual ~IListener() = default;
		virtual void Notify(Event* event) = 0;

	protected:
		virtual void SubscribeToEvent(std::initializer_list<EventId> events)
		{
			EventBroadcaster::Subscribe(this, events);
		}
		virtual void UnsubscribeFromEvent(std::initializer_list<EventId> events)
		{
			EventBroadcaster::Unsubscribe(this, events);
		}
	};

	// this does not need to subscribe to any specific events or eventSystem
	// only to the CollisionSystem!
	class ICollisionListener
	{
	public:
		virtual ~ICollisionListener() = default;
		virtual void OnCollision(const GameObject *const other) = 0;
		virtual void OnCollisionEntry(const GameObject *const other) = 0;
		virtual void OnCollisionExit(const GameObject *const other) = 0;
	};
}