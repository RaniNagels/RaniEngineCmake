#pragma once
#include <Components/Component.h>
#include <glm/glm.hpp>
#include <Components/IListener.h>
#include <memory>

namespace REC
{
	class Scene;
	class GameObject;
	class AnimatedSpriteComponent;
	class SpriteRenderComponent;
}

// TODO: change namespace
namespace Game
{
	class GridComponent;

	struct BombDescriptor
	{
		float lifeTime{ 2.f };
		GridComponent* grid{ nullptr };
		int explosionRange{ 5 };
		REC::Scene* scene{ nullptr };
	};

	class BombComponent final : public REC::Component, public REC::IListener
	{
	public:
		explicit BombComponent(REC::GameObject* owner, const BombDescriptor& descriptor);
		~BombComponent();

		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;
		BombComponent& operator=(const BombComponent& other) = delete;
		BombComponent& operator=(BombComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		void Detonate(); // can also be called by the player when in possession of a powerup
		void Reset(); // for reuse of bomb objects in allocator pools

		virtual void Notify(REC::Event* event) override;

	private:
		glm::uvec4 GetExplosionRange() const; // left, top, right, bottom
		void CreateExplosionInCell(REC::Scene* scene, REC::GameObject* root, glm::vec2 offset, bool end = false);
		uint8_t ComputeDirection(int rowStep, int colStep, REC::Event* destructionEvent) const;

		const BombDescriptor m_Descriptor;
		REC::AnimatedSpriteComponent* m_pAnimatedSpriteComponent{ nullptr };
		REC::SpriteRenderComponent* m_pSpriteRenderComponent{ nullptr };

		std::unique_ptr<REC::Event> m_pDetonateEvent{ nullptr };
		std::vector<std::unique_ptr<REC::Event>> m_DestructionEvents{}; // softblocks

		float m_Timer{0.f};
		const float m_LifeTime{ 2.f };
		bool m_Exploded{ false };
	};
}