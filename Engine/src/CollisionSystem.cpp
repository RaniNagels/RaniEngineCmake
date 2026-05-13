#include "CollisionSystem.h"

#include <Scene.h>
#include <Events/EventBroadcaster.h>
#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>

REC::CollisionSystem::~CollisionSystem() = default;

void REC::CollisionSystem::CheckCollisions(Scene* const currentScene)
{
	for (size_t i{}; i < m_CollidableObjects.size(); ++i)
	{
		if (!currentScene->Contains(m_CollidableObjects[i]->GetOwner()))
			continue;

		for (size_t j{ i + 1 }; j < m_CollidableObjects.size(); ++j)
		{
			if (!currentScene->Contains(m_CollidableObjects[j]->GetOwner()))
				continue;

			if (auto result{ FindCollision(m_CollidableObjects[i],  m_CollidableObjects[j]) })
			{
				if (result.has_value())
					SendCollisionEvent(m_CollidableObjects[i], m_CollidableObjects[j], CollisionEventType::OnOverlap);
			}
		}

		for (size_t j{}; j < m_StaticCollidableObjects.size(); ++j)
		{
			if (!currentScene->Contains(m_StaticCollidableObjects[j]->GetOwner()))
				continue;
			
			if (auto result{ FindCollision(m_CollidableObjects[i], m_StaticCollidableObjects[j]) })
			{
				if (result.has_value())
					SendCollisionEvent(m_CollidableObjects[i], m_StaticCollidableObjects[j], CollisionEventType::OnOverlap);
			}
		}
	}
}

void REC::CollisionSystem::Subscribe(CollisionComponent* subscriber)
{
	if (subscriber->IsStatic())
		m_StaticCollidableObjects.push_back(subscriber);
	else
		m_CollidableObjects.push_back(subscriber);
}

void REC::CollisionSystem::Unsubscribe(CollisionComponent* subscriber)
{
	if (subscriber->IsStatic())
		m_StaticCollidableObjects.erase(std::remove(m_StaticCollidableObjects.begin(), m_StaticCollidableObjects.end(), subscriber), m_StaticCollidableObjects.end());
	else
		m_CollidableObjects.erase(std::remove(m_CollidableObjects.begin(), m_CollidableObjects.end(), subscriber), m_CollidableObjects.end());
}

bool REC::CollisionSystem::HasCollidedWithStatic(CollisionComponent* comp1)
{
	for (const auto& staticComp : m_StaticCollidableObjects)
	{
		if (FindCollision(comp1, staticComp).has_value())
			return true;
	}
	
	return false;
}

bool REC::CollisionSystem::WillCollideWithStatic(CollisionComponent* comp1, const glm::vec2& movement)
{
	for (const auto& staticComp : m_StaticCollidableObjects)
	{
		if (FindCollision(comp1, staticComp, movement).has_value())
			return true;
	}

	return false;
}

void REC::CollisionSystem::SendCollisionEvent(CollisionComponent* comp1, CollisionComponent* comp2, CollisionEventType type)
{
	CollisionEvent collisionEvent(type, {});
	collisionEvent.SetCollidedObjects(comp1->GetOwner(), comp2->GetOwner());
	EventBroadcaster::Broadcast(collisionEvent);
}

std::optional<std::pair<const REC::Rect&, const REC::Rect&>> REC::CollisionSystem::FindCollision(CollisionComponent* comp1, CollisionComponent* comp2, const glm::vec2& offset1, const glm::vec2& offset2)
{
	auto worldPos1 = comp1->GetOwner()->GetTransform()->GetWorldPosition() + glm::vec3(offset1, 0.f);
	auto worldPos2 = comp2->GetOwner()->GetTransform()->GetWorldPosition() + glm::vec3(offset2, 0.f);

	for (const auto& bound1 : comp1->GetBounds())
	{
		for (const auto& bound2 : comp2->GetBounds())
		{
			if (worldPos1.x + bound1.rect.x < worldPos2.x + bound2.rect.x + bound2.rect.width &&
				worldPos1.x + bound1.rect.x + bound1.rect.width > worldPos2.x + bound2.rect.x &&
				worldPos1.y + bound1.rect.y < worldPos2.y + bound2.rect.y + bound2.rect.height &&
				worldPos1.y + bound1.rect.y + bound1.rect.height > worldPos2.y + bound2.rect.y)
			{
				return std::pair<const Rect&, const Rect&>(bound1.rect, bound2.rect);
			}
		}
	}
	return {};
}
