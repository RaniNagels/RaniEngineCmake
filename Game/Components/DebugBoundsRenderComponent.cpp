#include "DebugBoundsRenderComponent.h"
#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>
#include <GameObject.h>
#include <IRenderer.h>
#include <stdexcept>

Game::DebugBoundsRenderComponent::DebugBoundsRenderComponent(REC::GameObject* owner, REC::IRenderer* renderer, const REC::Color& color)
	: REC::RenderComponent(owner)
	, m_pCollisionComponent(GetOwner()->GetCollisionComponent())
	, m_pRenderer(renderer)
	, m_Color(color)
{
	if (m_pCollisionComponent == nullptr)
		throw std::invalid_argument("DebugBoundsRenderComponent requires a valid CollisionComponent on the same GameObject!");

	if (m_pRenderer == nullptr)
		throw std::invalid_argument("DebugBoundsRenderComponent requires a valid IRenderer pointer!");
}

void Game::DebugBoundsRenderComponent::Render()
{
	if (!m_pCollisionComponent) return;

	auto pos = GetOwner()->GetTransform()->GetWorldPosition();
	for (const auto& bounds : m_pCollisionComponent->GetBounds())
	{
		REC::Rect boundsWithPos{
			pos.x + bounds.x,
			pos.y + bounds.y,
			bounds.width,
			bounds.height
		};
		m_pRenderer->RenderRect(m_Color, boundsWithPos, false);
	}
}
