#pragma once
#include <Components/CollisionComponent.h>

namespace Game
{
	class BombComponent;
	class BalloomCollisionComponent final : public REC::CollisionComponent
	{
	public:
		explicit BalloomCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& descriptor);
		virtual ~BalloomCollisionComponent() = default;

		BalloomCollisionComponent(const BalloomCollisionComponent&) = delete;
		BalloomCollisionComponent(BalloomCollisionComponent&&) = delete;
		BalloomCollisionComponent& operator=(const BalloomCollisionComponent&) = delete;
		BalloomCollisionComponent& operator=(BalloomCollisionComponent&&) = delete;

		virtual void OnCollision(const REC::GameObject* const other) override;
	};
}