#pragma once
#include <sdbm_hash.h>
#include <vector>
#include <Events/EventBroadcaster.h>
#include <memory>

namespace REC
{
	using EventId = unsigned int;
	class IListener;
	class GameObject;

	// can be derived from to add more info about the event
	struct EventArgs
	{
		GameObject* sender{ nullptr };
	};

	class Event 
	{
	public:
		explicit Event(GameObject *const sender, EventId id);
		virtual ~Event() = default;

		Event(const Event& other) = delete;
		Event(Event&& other) = delete;
		Event& operator=(const Event& other) = delete;
		Event& operator=(Event&& other) = delete;

		bool IsEvent(EventId event) { return event == m_Id;}
		EventId GetId() const { return m_Id; }

		void Broadcast();

		bool IsSender(const GameObject* sender) const { return sender == m_pArgs->sender; }

	private:
		const EventId m_Id;
		EventBroadcaster m_EventBroadcaster{};
		std::unique_ptr<EventArgs> m_pArgs;
	};
}