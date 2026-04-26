#pragma once
#include <vector>
#include <memory>
#include <Events/Event.h>

namespace REC
{
	class CollisionComponent;
	class Scene;

	class CollisionSystem final
	{
	public:
		explicit CollisionSystem() = default;
		~CollisionSystem();

		CollisionSystem(const CollisionSystem&) = delete;
		CollisionSystem(CollisionSystem&&) = delete;
		CollisionSystem& operator=(const CollisionSystem&) = delete;
		CollisionSystem& operator=(CollisionSystem&&) = delete;

		void CheckCollisions(Scene *const currentScene);
		void Subscribe(CollisionComponent* subscriber);
		void Unsubscribe(CollisionComponent* subscriber);

	private:
		void SendCollisionEvent(CollisionComponent* comp1, CollisionComponent* comp2, CollisionEventType type);
		bool CheckCollision(CollisionComponent* comp1, CollisionComponent* comp2);

		std::vector<CollisionComponent*> m_CollidableObjects{};
		std::vector<CollisionComponent*> m_StaticCollidableObjects{};
		std::vector<std::unique_ptr<CollisionEvent>> m_CollisionEvents{};
	};
}