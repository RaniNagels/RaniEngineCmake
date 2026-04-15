#pragma once
#include <vector>
#include <memory>

namespace REC
{
	using EventId = unsigned int;
	class IListener;
	class GameObject;

	// can be derived from to add more info about the event
	struct EventArgs
	{
		virtual ~EventArgs() = default;
		virtual std::unique_ptr<EventArgs> makeUnique() const
		{
			return std::make_unique<EventArgs>(*this);
		}
	};

	struct GameObjectEventArgs : public EventArgs
	{
		GameObject* sender{ nullptr };

		virtual std::unique_ptr<EventArgs> makeUnique() const override
		{
			return std::make_unique<GameObjectEventArgs>(*this);
		}
	};

	enum class CollisionEventType : uint8_t
	{
		None,
		OnEntry,
		OnExit,
		OnOverlap
	};

	struct CollisionEventArgs : public EventArgs
	{
		GameObject* object1{ nullptr };
		GameObject* object2{ nullptr };

		CollisionEventType type{};

		virtual std::unique_ptr<EventArgs> makeUnique() const override
		{
			return std::make_unique<CollisionEventArgs>(*this);
		}
	};

	class Event 
	{
	public:
		explicit Event(EventId id, const EventArgs& args);
		virtual ~Event() = default;

		Event(const Event& other) = delete;
		Event(Event&& other) = delete;
		Event& operator=(const Event& other) = delete;
		Event& operator=(Event&& other) = delete;

		bool IsEvent(EventId event) { return event == m_Id;}
		EventId GetId() const { return m_Id; }
		EventArgs* GetArgs() const { return m_pArgs.get(); }

		void Broadcast();

	private:
		const EventId m_Id;
		std::unique_ptr<EventArgs> m_pArgs;
	};
}