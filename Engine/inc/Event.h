#pragma once
#include <sdbm_hash.h>
#include <vector>

namespace REC
{
	using EventId = unsigned int;
	class IListener;

	class Event 
	{
	public:
		explicit Event(EventId id);
		virtual ~Event() = default;

		Event(const Event& other) = delete;
		Event(Event&& other) = delete;
		Event& operator=(const Event& other) = delete;
		Event& operator=(Event&& other) = delete;

		bool IsEvent(EventId event) { return event == m_Id;}

		void Subscribe(IListener* listener);
		void Unsubscribe(IListener* listener);
		void NotifyListeners();

	private:
		const EventId m_Id;
		std::vector<IListener*> m_Listeners;
	};

	class ValueChangedEvent : public Event
	{
	public:
		explicit ValueChangedEvent(EventId id)
			:Event(id){ }
		virtual ~ValueChangedEvent() = default;

		ValueChangedEvent(const ValueChangedEvent& other) = delete;
		ValueChangedEvent(ValueChangedEvent&& other) = delete;
		ValueChangedEvent& operator=(const ValueChangedEvent& other) = delete;
		ValueChangedEvent& operator=(ValueChangedEvent&& other) = delete;

		void SetDelta(float delta) { m_Delta = delta; }
		float GetDelta() { return m_Delta; }

	private:
		float m_Delta{ 0 };
	};
}