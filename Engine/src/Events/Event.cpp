#include <Events/Event.h>
#include <Components/IListener.h>
#include <Events/EventBroadcaster.h>

REC::Event::Event(GameObject* const sender, EventId id)
	:m_Id{ id }
	, m_pArgs{ std::make_unique<EventArgs>() }
{ 
	m_pArgs->sender = sender;
}

void REC::Event::Broadcast()
{
	EventBroadcaster::Broadcast(this);
}
