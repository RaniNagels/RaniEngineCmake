#pragma once
#include <Components/Component.h>

// TODO: change namespace
namespace Game
{
	class BombComponent final : public REC::Component
	{
	public:
		explicit BombComponent(GameObject* owner);
		~BombComponent() = default;

		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;
		BombComponent& operator=(const BombComponent& other) = delete;
		BombComponent& operator=(BombComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		void Detonate();

	private:
		float m_LifeTime{ 2.f };

	};
}