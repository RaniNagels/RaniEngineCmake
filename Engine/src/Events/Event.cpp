#include <Events/Event.h>
#include <Components/IListener.h>
#include <Events/EventBroadcaster.h>


REC::Event::Event(EventId id, const EventArgs& args)
	: m_Id{id}
	, m_pArgs{args.makeUnique()}
{
}

void REC::Event::Broadcast()
{
	EventBroadcaster::Broadcast(this);
}
