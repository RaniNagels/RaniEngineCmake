#pragma once
#include <Components/RenderComponent.h>
#include <RECColor.h>
#include <string>

namespace REC
{
	class IRenderer;
}

namespace Game
{
	class GridComponent;

	// depends on the GridComponent to work
	// no grid component -> error
	class DebugGridRenderComponent final : public REC::RenderComponent
	{
	public:
		// TODO: find a different way of passing the renderer (a static in the base class?)
		explicit DebugGridRenderComponent(REC::GameObject* owner, const REC::Color& color = { 255, 255, 255, 255 });
		virtual ~DebugGridRenderComponent() = default;

		DebugGridRenderComponent(const DebugGridRenderComponent& other) = delete;
		DebugGridRenderComponent(DebugGridRenderComponent&& other) = delete;
		DebugGridRenderComponent& operator=(const DebugGridRenderComponent& other) = delete;
		DebugGridRenderComponent& operator=(DebugGridRenderComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		virtual void Render(const REC::IRenderer *const renderer) override;

	private:
		GridComponent* m_pGridComponent;
		REC::Color m_Color;
	};
}