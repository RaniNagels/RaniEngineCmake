#include <Event.h>
#include <Components/IListener.h>

REC::Event::Event(EventId id)
	:m_Id{ id }
	, m_Listeners{}
{ 
}

void REC::Event::Subscribe(IListener* listener)
{
	m_Listeners.emplace_back(listener);
}

void REC::Event::Unsubscribe(IListener* listener)
{
	std::erase(m_Listeners, listener);
}

void REC::Event::NotifyListeners()
{
	for (auto* listener : m_Listeners)
		listener->Notify(this);
}
