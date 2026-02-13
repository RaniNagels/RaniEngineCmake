#include "../inc/SpriteRenderComponent.h"
#include "../Renderer.h"
#include "../inc/TransformComponent.h"
#include "../GameObject.h"
#include "../ResourceManager.h"

REC::SpriteRenderComponent::SpriteRenderComponent(Texture2D* texture)
	: m_pTexture(texture)
{
}

REC::SpriteRenderComponent::SpriteRenderComponent(const std::string& textureName)
{
	SetTexture(textureName);
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

void REC::SpriteRenderComponent::SetTexture(const std::string& textureName)
{
	// TODO: maybe use messenger system here. send request for resource and receive resource
	// prevent using resourcemanager directly in this class
	m_pTexture = ResourceManager::GetInstance().GetResource<Texture2D>(textureName);
}
