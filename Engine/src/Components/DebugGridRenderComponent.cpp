#include "../../inc/Components/DebugGridRenderComponent.h"
#include "../../inc/GameObject.h"
#include "../../inc/Components/GridComponent.h"
#include "../../inc/Components/TransformComponent.h"
#include <stdexcept>
#include "../Renderer.h"

REC::DebugGridRenderComponent::DebugGridRenderComponent(GameObject* owner, const Color& color)
	: RenderComponent(owner)
	, m_Color{ color }
{
	if (!GetOwner()->HasComponent<GridComponent>())
		throw std::runtime_error("GameObject does not have a gridComponent!");
	else
		m_pGridComponent = GetOwner()->GetComponent<GridComponent>();
}

void REC::DebugGridRenderComponent::Update(float) {}

void REC::DebugGridRenderComponent::Render()
{
	auto data = m_pGridComponent->GetDescriptorData();
	auto& R = Renderer::GetInstance();
	auto worldPos = GetOwner()->GetTransform()->GetWorldPosition();

	Rect rect{};
	rect.width = data.cellWidth;
	rect.height = data.cellHeight;

	for (uint8_t r{}; r < data.rows; ++r)
	{
		for (uint8_t c{}; c < data.cols; ++c)
		{
			rect.x = worldPos.x + (c * data.cellWidth);
			rect.y = worldPos.y + (r * data.cellHeight);
			R.RenderRect(m_Color, rect);
		}
	}
}