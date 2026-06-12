#pragma once
#include <Components/Component.h>
#include <Components/IListener.h>
#include <glm/glm.hpp>

namespace REC
{
	class Scene;
}

namespace Game
{
	class GridComponent;
	class EnemyMovementComponent : public REC::Component, public REC::IListener
	{
	public:
		explicit EnemyMovementComponent(REC::GameObject* owner, GridComponent* grid, REC::Scene* scene, float speed);
		virtual ~EnemyMovementComponent();

		EnemyMovementComponent(const EnemyMovementComponent&) = delete;
		EnemyMovementComponent(EnemyMovementComponent&&) noexcept = delete;
		EnemyMovementComponent& operator=(const EnemyMovementComponent&) = delete;
		EnemyMovementComponent& operator=(EnemyMovementComponent&&) noexcept = delete;

		virtual void Update(float deltaT) override;
		virtual void Notify(REC::Event* event) override;

	private:
		void FindNewPosition();

		float m_Speed{ 50.f };
		GridComponent* m_pGrid{ nullptr };
		REC::Scene* m_pScene{ nullptr };

		glm::vec2 m_TargetPosition{};
	};
}