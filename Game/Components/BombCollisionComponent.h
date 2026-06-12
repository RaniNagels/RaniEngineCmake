#pragma once
#include <Components/CollisionComponent.h>

namespace Game
{
	class BombComponent;
	class BombCollisionComponent final : public REC::CollisionComponent
	{
	public:
		explicit BombCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& descriptor);
		virtual ~BombCollisionComponent() = default;

		BombCollisionComponent(const BombCollisionComponent&) = delete;
		BombCollisionComponent(BombCollisionComponent&&) = delete;
		BombCollisionComponent& operator=(const BombCollisionComponent&) = delete;
		BombCollisionComponent& operator=(BombCollisionComponent&&) = delete;

		virtual void OnCollision(const REC::GameObject* const other) override;
	private:
		BombComponent* m_pBombComponent{ nullptr };
	};
}