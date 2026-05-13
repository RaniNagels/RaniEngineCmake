#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace REC
{
	class GameObject;
	class CollisionSystem;

	// RigidBodyComponents get resolved in this system
	// will only resolve movement for static vs non-static objects, not dynamic vs dynamic!!
	class PhysicsSystem final
	{
	public:
		explicit PhysicsSystem(CollisionSystem* CollisionSystem) : m_pCollisionSystem{ CollisionSystem } {}
		~PhysicsSystem() = default;

		PhysicsSystem(const PhysicsSystem&) = delete;
		PhysicsSystem(PhysicsSystem&&) noexcept = delete;
		PhysicsSystem& operator=(const PhysicsSystem&) = delete;
		PhysicsSystem& operator=(PhysicsSystem&&) noexcept = delete;

		void Update(float deltaTime);

		void AddPhysicsObject(GameObject* physicsObject);
		void RemovePhysicsObject(GameObject* physicsObject);

	private:
		std::vector<GameObject*> m_PhysicsObjects{}; // must contain a rigidBoyComponent

		void ResolveMovement(GameObject* obj, size_t index, const glm::vec2& movement);
		CollisionSystem* m_pCollisionSystem{ nullptr };
	};
}