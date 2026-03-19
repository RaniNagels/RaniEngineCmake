#include "DebugGridRenderComponent.h"
#include <GameObject.h>
#include "GridComponent.h"
#include <Components/TransformComponent.h>
#include <stdexcept>
#include <IRenderer.h>

Game::DebugGridRenderComponent::DebugGridRenderComponent(REC::GameObject* owner, REC::IRenderer* renderer, const REC::Color& color)
	: RenderComponent(owner)
	, m_pRenderer{renderer}
	, m_Color{ color }
{
	if (!GetOwner()->HasComponent<GridComponent>())
		throw std::runtime_error("GameObject does not have a gridComponent!");
	else
		m_pGridComponent = GetOwner()->GetComponent<GridComponent>();
}

void Game::DebugGridRenderComponent::Update(float) {}

void Game::DebugGridRenderComponent::Render()
{
	auto data = m_pGridComponent->GetDescriptorData();
	auto worldPos = GetOwner()->GetTransform()->GetWorldPosition();

	REC::Rect rect{};
	rect.width = data.cellWidth;
	rect.height = data.cellHeight;

	for (uint8_t r{}; r < data.rows; ++r)
	{
		for (uint8_t c{}; c < data.cols; ++c)
		{
			rect.x = worldPos.x + (c * data.cellWidth);
			rect.y = worldPos.y + (r * data.cellHeight);
			m_pRenderer->RenderRect(m_Color, rect);
		}
	}
}