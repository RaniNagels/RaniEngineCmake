#include "DebugGridRenderComponent.h"
#include <GameObject.h>
#include "GridComponent.h"
#include <Components/TransformComponent.h>
#include <stdexcept>
#include <IRenderer.h>

Game::DebugGridRenderComponent::DebugGridRenderComponent(REC::GameObject* owner, const REC::Color& color)
	: RenderComponent(owner)
	, m_Color{ color }
{
	if (!GetOwner()->HasComponent<GridComponent>())
		throw std::runtime_error("GameObject does not have a gridComponent!");
	else
		m_pGridComponent = GetOwner()->GetComponent<GridComponent>();
}

void Game::DebugGridRenderComponent::Update(float) {}

void Game::DebugGridRenderComponent::Render(const REC::IRenderer* const renderer)
{
	const std::vector<Game::GridComponent::Cell>& cells = m_pGridComponent->GetCells();

	for (const auto& cell : cells)
	{
		renderer->RenderRect(m_Color, cell.GetRect(), cell.isWall);
	}
}