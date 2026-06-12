#pragma once
#include <Components/CollisionComponent.h>

namespace REC
{
	class Scene;
}

namespace Game
{
	class DoorCollisionComponent final : public REC::CollisionComponent
	{
	public:
		explicit DoorCollisionComponent(REC::GameObject* owner, const REC::CollisionDescriptor& desc, REC::Scene* scene);
		virtual ~DoorCollisionComponent() = default;

		DoorCollisionComponent(const DoorCollisionComponent& other) = delete;
		DoorCollisionComponent(DoorCollisionComponent&& other) = delete;
		DoorCollisionComponent& operator=(const DoorCollisionComponent& other) = delete;
		DoorCollisionComponent& operator=(DoorCollisionComponent&& other) = delete;

		virtual void OnCollision(const REC::GameObject* const other) override;

	private:
		std::unique_ptr<REC::Event> m_pLevelChangeEvent{ nullptr };
		REC::Scene* m_pScene{ nullptr };
	};
}