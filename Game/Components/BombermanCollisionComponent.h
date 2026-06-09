#pragma once
#include <Components/CollisionComponent.h>
#include <vector>
#include <Events/Event.h>
#include <memory>

namespace Game
{
	class BombermanCollisionComponent final : public REC::CollisionComponent
	{
	public:
		explicit BombermanCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& descriptor);
		virtual ~BombermanCollisionComponent() = default;

		BombermanCollisionComponent(const BombermanCollisionComponent&) = delete;
		BombermanCollisionComponent(BombermanCollisionComponent&&) = delete;
		BombermanCollisionComponent& operator=(const BombermanCollisionComponent&) = delete;
		BombermanCollisionComponent& operator=(BombermanCollisionComponent&&) = delete;

		virtual void OnCollision(const REC::GameObject *const other) override;
		virtual void OnCollisionEntry(const REC::GameObject *const other) override;
		virtual void OnCollisionExit(const REC::GameObject *const other) override;
	};
}