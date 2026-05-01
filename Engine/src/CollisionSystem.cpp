#include "CollisionSystem.h"
#include <Scene.h>
#include <Components/CollisionComponent.h>
#include <Events/EventBroadcaster.h>
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

			//if (CheckCollision(m_CollidableObjects[i], m_CollidableObjects[j]))
			//	SendCollisionEvent(m_CollidableObjects[i], m_CollidableObjects[j], CollisionEventType::OnOverlap);
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

			//if (CheckCollision(m_CollidableObjects[i], m_StaticCollidableObjects[j]))
			//	SendCollisionEvent(m_CollidableObjects[i], m_StaticCollidableObjects[j], CollisionEventType::OnOverlap);
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

void REC::CollisionSystem::SendCollisionEvent(CollisionComponent* comp1, CollisionComponent* comp2, CollisionEventType type)
{
	CollisionEvent collisionEvent(type, {});
	collisionEvent.SetCollidedObjects(comp1->GetOwner(), comp2->GetOwner());
	EventBroadcaster::Broadcast(collisionEvent);
}

bool REC::CollisionSystem::CheckCollision(CollisionComponent* comp1, CollisionComponent* comp2)
{
	auto worldPos1 = comp1->GetOwner()->GetTransform()->GetWorldPosition();
	auto worldPos2 = comp2->GetOwner()->GetTransform()->GetWorldPosition();

	for (const auto& bound1 : comp1->GetBounds())
	{
		for (const auto& bound2 : comp2->GetBounds())
		{
			if (worldPos1.x + bound1.x < worldPos2.x + bound2.x + bound2.width &&
				worldPos1.x + bound1.x + bound1.width > worldPos2.x + bound2.x &&
				worldPos1.y + bound1.y < worldPos2.y + bound2.y + bound2.height &&
				worldPos1.y + bound1.y + bound1.height > worldPos2.y + bound2.y)
			{
				return true;
			}
		}
	}

	return false;
}

std::optional<std::pair<const REC::Rect&, const REC::Rect&>> REC::CollisionSystem::FindCollision(CollisionComponent* comp1, CollisionComponent* comp2)
{
	auto worldPos1 = comp1->GetOwner()->GetTransform()->GetWorldPosition();
	auto worldPos2 = comp2->GetOwner()->GetTransform()->GetWorldPosition();

	for (const auto& bound1 : comp1->GetBounds())
	{
		for (const auto& bound2 : comp2->GetBounds())
		{
			if (worldPos1.x + bound1.x < worldPos2.x + bound2.x + bound2.width &&
				worldPos1.x + bound1.x + bound1.width > worldPos2.x + bound2.x &&
				worldPos1.y + bound1.y < worldPos2.y + bound2.y + bound2.height &&
				worldPos1.y + bound1.y + bound1.height > worldPos2.y + bound2.y)
			{
				return std::pair<const Rect&, const Rect&>(bound1, bound2);
			}
		}
	}
	return {};
}
