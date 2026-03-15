#pragma once
#include <vector>
#include <string>

namespace REC
{
	class Event;

	// component class can also inherit from this
	// only components can have listeners (observers), no GameObject should have them!
	class IListener // aka Observer
	{
	public:
		virtual ~IListener() = default;
		void Notify(Event* event) = 0;
	};

	// not an interface, cannot be inherited by a component
	// components own there events
	// subject are not components (kind of possible, but not recommended)
	class Dispatcher
	{
	public:
		virtual ~Dispatcher() = default;
		void Subscribe(IListener* listener)   { m_Listeners.emplace_back(listener); }
		void Unsubscribe(IListener* listener) { std::remove(m_Listeners.begin(), m_Listeners.end(), listener); }

	protected:
		void Dispatch(Event* event)
		{
			for (auto* listener : m_Listeners)
				listener->Notify(event);
		}

	private:
		// non-owning list!
		std::vector<IListener*> m_Listeners;
	};

	// final, pas arguments as a struct
	class Event final: public Dispatcher
	{
	public:
		virtual ~Event() = default;
		void NotifyListeners() { Dispatch(this); }

	private:
		const std::string m_ID; // TODO: use hash
	};
}