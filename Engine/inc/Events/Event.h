#pragma once
#include <vector>
#include <memory>
#include <stdexcept>

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
		virtual ~GameObjectEventArgs() = default;
	};

	enum class CollisionEventType : uint8_t
	{
		OnEntry,
		OnExit,
		OnOverlap
	};

	class Event 
	{
	public:
		explicit Event(EventId id, const EventArgs& args);
		virtual ~Event() = default;

		Event(const Event& other)
			: m_Id{other.m_Id}
			, m_pArgs{other.m_pArgs->makeUnique()}
		{
		}
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

	class CollisionEvent : public Event
	{
	public:
		explicit CollisionEvent(CollisionEventType type, const EventArgs& args = {});
		virtual ~CollisionEvent() = default;

		CollisionEvent(const CollisionEvent& other)
			: Event(other)
			, m_Object1{ other.m_Object1 }
			, m_Object2{ other.m_Object2 }
			, m_Type{ other.m_Type }
		{
		}
		CollisionEvent(CollisionEvent&& other) = delete;
		CollisionEvent& operator=(const CollisionEvent& other) = delete;
		CollisionEvent& operator=(CollisionEvent&& other) = delete;

		void Broadcast();

		bool IsType(CollisionEventType type) const;
		bool HasCollidedWith(GameObject* object) const;
		void SetCollidedObjects(GameObject* obj1, GameObject* obj2);
		GameObject* GetObject(GameObject* other) const
		{
			if (m_Object1 == other)
				return m_Object2;
			else if (m_Object2 == other)
				return m_Object1;
			else
				return nullptr;
		}
		std::vector<GameObject*> GetCollidedObjects() const { return { m_Object1, m_Object2 }; }

	private:
		GameObject* m_Object1{ nullptr };
		GameObject* m_Object2{ nullptr };

		CollisionEventType m_Type;
	};
}