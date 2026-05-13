#pragma once
#include <Components/Component.h>

#include <glm/glm.hpp>

namespace REC
{
	class CollisionComponent;
	class PhysicsSystem;

	class RigidBodyComponent final : public Component
	{
	public:
		explicit RigidBodyComponent(GameObject* owner);
		virtual ~RigidBodyComponent();

		RigidBodyComponent(const RigidBodyComponent&) = delete;
		RigidBodyComponent(RigidBodyComponent&&) noexcept = delete;
		RigidBodyComponent& operator=(const RigidBodyComponent&) = delete;
		RigidBodyComponent& operator=(RigidBodyComponent&&) noexcept = delete;

		virtual void Update(float) override {};
		virtual void Destroy() override;

		virtual void SetVelocity(const glm::vec2& velocity);
		virtual void AddToVelocity(const glm::vec2& velocity);

		static void SetPhysicsSystem(PhysicsSystem* physicsSystem) { s_PhysicsSystem = physicsSystem; }

		glm::vec2 GetVelocity() const { return m_Velocity; }
		void ResetVelocity() { m_Velocity = {}; }

	private:
		glm::vec2 m_Velocity{};
		bool m_IsStatic{ true };

		static PhysicsSystem* s_PhysicsSystem;
	};
}