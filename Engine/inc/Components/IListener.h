#pragma once

namespace REC
{
	class Event;

	// component class can inherit from it!!
	class IListener // aka Observer
	{
	public:
		virtual ~IListener() = default;
		virtual void Notify(Event* event) = 0;
	};
}