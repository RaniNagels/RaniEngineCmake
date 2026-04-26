#include <Events/Event.h>
#include <Components/IListener.h>
#include <Events/EventBroadcaster.h>
#include <sdbm_hash.h>


REC::Event::Event(EventId id, const EventArgs& args)
	: m_Id{id}
	, m_pArgs{args.makeUnique()}
{
}

void REC::Event::Broadcast()
{
	EventBroadcaster::Broadcast(this);
}

REC::CollisionEvent::CollisionEvent(CollisionEventType type, const EventArgs& args)
	: Event(make_sdbm_hash("CollisionEvent"), args)
	, m_Type{ type }
{
}

bool REC::CollisionEvent::IsType(CollisionEventType type) const
{
	return m_Type == type;
}

bool REC::CollisionEvent::HasCollidedWith(GameObject* object) const
{
	return m_Object1 == object || m_Object2 == object;
}

void REC::CollisionEvent::SetCollidedObjects(GameObject* obj1, GameObject* obj2)
{
	m_Object1 = obj1;
	m_Object2 = obj2;
}