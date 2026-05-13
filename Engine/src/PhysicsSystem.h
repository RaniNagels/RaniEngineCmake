#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace REC
{
	class GameObject;
	class CollisionSystem;
	class RigidBodyComponent;

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

		void AddPhysicsObject(RigidBodyComponent* physicsObject);
		void RemovePhysicsObject(RigidBodyComponent* physicsObject);

	private:
		std::vector<RigidBodyComponent*> m_RigidBodies{}; 
		std::vector<RigidBodyComponent*> m_StaticRigidBodies{};

		void ResolveMovement(GameObject* obj, const glm::vec2& movement);
		CollisionSystem* m_pCollisionSystem{ nullptr };
	};
}