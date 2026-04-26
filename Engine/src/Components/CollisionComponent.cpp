#include <Components/CollisionComponent.h>
#include <Events/EventBroadcaster.h>
#include <GameObject.h>
#include <Components/TransformComponent.h>

REC::CollisionComponent::CollisionComponent(GameObject* owner, const CollisionDescriptor& descriptor)
	: Component(owner)
	, ICollisionListener()
	, m_Bounds(descriptor.bounds)
	, m_Descriptor(descriptor)
{
	if (IsStatic())
		GetOwner()->GetTransform()->SetStatic();
	EventBroadcaster::Subscribe(this);
}

void REC::CollisionComponent::Destroy()
{
	EventBroadcaster::Unsubscribe(this);
}

bool REC::CollisionComponent::IsStatic() const
{
	return m_Descriptor.collisionType == CollisionType::Static;
}