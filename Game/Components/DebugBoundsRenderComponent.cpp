#include "DebugBoundsRenderComponent.h"
#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>
#include <GameObject.h>
#include <IRenderer.h>
#include <stdexcept>

Game::DebugBoundsRenderComponent::DebugBoundsRenderComponent(REC::GameObject* owner, const REC::Color& color)
	: REC::RenderComponent(owner)
	, m_pCollisionComponent(GetOwner()->GetCollisionComponent())
	, m_Color(color)
{
	if (m_pCollisionComponent == nullptr)
		throw std::invalid_argument("DebugBoundsRenderComponent requires a valid CollisionComponent on the same GameObject!");
}

void Game::DebugBoundsRenderComponent::Render(const REC::IRenderer *const renderer)
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
		renderer->RenderRect(m_Color, boundsWithPos, false);
	}
}
