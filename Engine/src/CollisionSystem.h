#pragma once
#include <vector>
#include <memory>
#include <Events/Event.h>

namespace REC
{
	class GameObject;
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

		void CheckCollisions(Scene *const currentScene, float deltaTime);

	private:
		std::vector<GameObject*> m_CollidableObjects{};
		std::vector<std::unique_ptr<Event>> m_CollisionEvents{};
	};
}