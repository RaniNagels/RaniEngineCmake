#pragma once
#include <Events/EventBroadcaster.h>

namespace REC
{
	class Event;

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
}