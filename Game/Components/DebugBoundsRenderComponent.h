#pragma once
#include <Components/RenderComponent.h>
#include <RECColor.h>

namespace REC
{
	class GameObject;
	class CollisionComponent;
	class IRenderer;
}

namespace Game
{
	class DebugBoundsRenderComponent final : public REC::RenderComponent
	{
	public:
		// TODO: find a different way of passing the renderer (a static in the base class?)
		DebugBoundsRenderComponent(REC::GameObject* owner, REC::IRenderer* renderer, const REC::Color& color = { 255, 255, 255, 255 });
		~DebugBoundsRenderComponent() = default;

		DebugBoundsRenderComponent(const DebugBoundsRenderComponent&) = delete;
		DebugBoundsRenderComponent(DebugBoundsRenderComponent&&) = delete;
		DebugBoundsRenderComponent& operator=(const DebugBoundsRenderComponent&) = delete;
		DebugBoundsRenderComponent& operator=(DebugBoundsRenderComponent&&) = delete;

		virtual void Render() override;
		virtual void Update(float) override {};

	private:
		REC::CollisionComponent* m_pCollisionComponent;
		REC::IRenderer* m_pRenderer;
		REC::Color m_Color;
	};
}