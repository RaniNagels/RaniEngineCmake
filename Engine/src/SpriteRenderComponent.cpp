#include "../inc/SpriteRenderComponent.h"
#include "../Renderer.h"
#include "../inc/TransformComponent.h"
#include "../GameObject.h"

REC::SpriteRenderComponent::SpriteRenderComponent(Texture2D* texture)
	: m_pTexture(texture)
{
}

void REC::SpriteRenderComponent::Update(float)
{
}

void REC::SpriteRenderComponent::Render()
{
	auto* transform = this->GetOwner()->GetComponent<TransformComponent>();
	glm::vec2 pos{};
	if (transform) pos = transform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}
