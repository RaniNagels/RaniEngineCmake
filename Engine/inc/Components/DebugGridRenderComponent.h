#pragma once
#include "RenderComponent.h"
#include "../RECColor.h"

namespace REC
{
	class GridComponent;

	// depends on the GridComponent to work
	// no grid component -> error
	class DebugGridRenderComponent final : public RenderComponent
	{
	public:
		explicit DebugGridRenderComponent(GameObject* owner, const Color& color = { 255, 255, 255, 255 });
		virtual ~DebugGridRenderComponent() = default;

		DebugGridRenderComponent(const DebugGridRenderComponent& other) = delete;
		DebugGridRenderComponent(DebugGridRenderComponent&& other) = delete;
		DebugGridRenderComponent& operator=(const DebugGridRenderComponent& other) = delete;
		DebugGridRenderComponent& operator=(DebugGridRenderComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		virtual void Render() override;

	private:
		GridComponent* m_pGridComponent;
		Color m_Color;
	};
}