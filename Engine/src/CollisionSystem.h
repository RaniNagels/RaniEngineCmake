#pragma once
#include <vector>
#include <memory>
#include <optional>

#include <Events/Event.h>
#include <GeneralStructs.h>

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

		bool HasCollidedWithStatic(CollisionComponent* comp1);
		bool WillCollide(CollisionComponent* comp1, CollisionComponent* comp2, const glm::vec2& movement1, const glm::vec2& movement2);

	private:
		void SendCollisionEvent(CollisionComponent* comp1, CollisionComponent* comp2, CollisionEventType type);
		std::optional<std::pair<const Rect&, const Rect&>> FindCollision(CollisionComponent* comp1, CollisionComponent* comp2, const glm::vec2& offset1 = {}, const glm::vec2& offset2 = {});

		std::vector<CollisionComponent*> m_CollidableObjects{};
		std::vector<CollisionComponent*> m_StaticCollidableObjects{};
		std::vector<std::unique_ptr<CollisionEvent>> m_CollisionEvents{};
	};
}