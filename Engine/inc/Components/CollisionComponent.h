#pragma once
#include <Components/IListener.h>
#include <Components/Component.h>
#include <GeneralStructs.h>
#include <vector>
#include <ComponentDescriptors.h>

namespace REC
{
	class GameObject;
	class CollisionEvent;

	// a base Collision component that contains data for for the collision to work 
	// as well as a listener for collision events involving itself
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

		const std::vector<Rect>& GetBounds() const { return m_Bounds; }

		bool IsStatic() const;

	private:
		std::vector<Rect> m_Bounds{}; // a vector allows for multiple collisionboxes per object allowing for more complex shapes
		const CollisionDescriptor m_Descriptor;
	};
}