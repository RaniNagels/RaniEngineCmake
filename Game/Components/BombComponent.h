#pragma once
#include <Components/Component.h>

namespace REC
{
	class GameObject;
	class AnimatedSpriteComponent;
	class SpriteRenderComponent;
}

// TODO: change namespace
namespace Game
{
	struct BombDescriptor
	{
		float lifeTime{ 2.f };
	};

	class BombComponent final : public REC::Component
	{
	public:
		explicit BombComponent(REC::GameObject* owner, const BombDescriptor& descriptor);
		~BombComponent() = default;

		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;
		BombComponent& operator=(const BombComponent& other) = delete;
		BombComponent& operator=(BombComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		void Detonate(); // can also be called by the player when in possession of a powerup
		void Reset(); // for reuse of bomb objects in allocator pools

	private:
		const BombDescriptor m_Descriptor;
		REC::AnimatedSpriteComponent* m_pAnimatedSpriteComponent{ nullptr };
		REC::SpriteRenderComponent* m_pSpriteRenderComponent{ nullptr };

		float m_Timer{0.f};
		const float m_LifeTime{ 2.f };
		bool m_Exploded{ false };
	};
}