#pragma once
#include <vector>

#include <GeneralStructs.h>
#include <ComponentDescriptors.h>
#include <Components/IListener.h>
#include <Components/Component.h>

namespace REC
{
	class GameObject;
	class CollisionEvent;

	// a base Collision component that contains data for for the collision to work 
	// as well as a listener for collision events involving itself
	// != rigid body. rigid body resolves collision
	class CollisionComponent : public Component, public ICollisionListener
	{
	public:
		explicit CollisionComponent(GameObject* owner, const CollisionDescriptor& descriptor);
		virtual ~CollisionComponent() = default;

		CollisionComponent(const CollisionComponent&) = delete;
		CollisionComponent(CollisionComponent&&) = delete;
		CollisionComponent& operator=(const CollisionComponent&) = delete;
		CollisionComponent& operator=(CollisionComponent&&) = delete;

		virtual void OnCollision(const GameObject *const) override {}
		virtual void OnCollisionEntry(const GameObject *const) override {}
		virtual void OnCollisionExit(const GameObject *const) override {}

		virtual void Destroy() override;
		virtual void Update(float) override {};

		const std::vector<CollisionBound>& GetBounds() const { return m_Bounds; }
		const std::vector<CollisionBound>& GetTriggerBounds() const
		{
			std::vector<CollisionBound> triggerBounds{};
			for (const auto& bound : m_Bounds)
			{
				if (bound.isTrigger)
					triggerBounds.emplace_back(bound);
			}
			return triggerBounds;
		}

		std::vector<CollisionBound>& GetModifiableBounds() { return m_Bounds; }

		bool IsStatic() const;
		bool Enabled() const { return m_Enabled; }
		void ToggleEnabled() { m_Enabled = !m_Enabled; }

	private:
		std::vector<CollisionBound> m_Bounds{}; // a vector allows for multiple collisionboxes per object allowing for more complex shapes
		const CollisionDescriptor m_Descriptor;
		bool m_Enabled{ true };
	};
}