#include <Events/EventSystem.h>
#include <Events/Event.h>
#include <Components/IListener.h>
#include <sdbm_hash.h>

void REC::EventSystem::ProcessEvents()
{
	while (!m_EventQueue.empty())
	{
		Event* event = m_EventQueue.front();
		m_EventQueue.pop();

		EventId id = event->GetId();
		if (m_EventSubscribers.find(id) != m_EventSubscribers.end())
		{
			for (IListener* listener : m_EventSubscribers[id])
			{
				listener->Notify(event);
			}
		}
	}
}

void REC::EventSystem::BroadcastEvent(Event* event)
{
	m_EventQueue.push(event);
}

void REC::EventSystem::Subscribe(IListener* subscriber, std::initializer_list<EventId> events)
{
	for (const unsigned int& event : events)
	{
		m_EventSubscribers[event].push_back(subscriber);
	}
}

void REC::EventSystem::Unsubscribe(IListener* subscriber, std::initializer_list<EventId> events)
{
	if (events.size() == 0) // remove from all events
	{
		for (auto& [event, subscribers] : m_EventSubscribers)
		{
			subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), subscriber), subscribers.end());
		}
	}
	else
	{
		for (const unsigned int& event : events)
		{
			if (m_EventSubscribers.find(event) != m_EventSubscribers.end())
			{
				m_EventSubscribers[event].erase(std::remove(m_EventSubscribers[event].begin(), m_EventSubscribers[event].end(), subscriber), m_EventSubscribers[event].end());
			}
		}
	}
}