#include "../../inc/Components/SpriteRenderComponent.h"
#include "../../inc/Components/TransformComponent.h"
#include "../../inc/GameObject.h"
#include "../../inc/ResourceManager.h"
#include "../Renderer.h"

REC::SpriteRenderComponent::SpriteRenderComponent(Texture2D* texture, uint16_t width, uint16_t height)
	: m_pTexture(texture)
	, m_DrawWidth{width}
	, m_DrawHeight{height}
{
}

REC::SpriteRenderComponent::SpriteRenderComponent(const std::string& textureName, uint16_t width, uint16_t height)
	: m_DrawWidth{width}
	, m_DrawHeight{height}
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

	glm::vec2 size{};
	auto originalSize = m_pTexture->GetSize();
	if (m_DrawHeight == 0 && m_DrawWidth == 0)
		size = originalSize;
	else if (m_DrawHeight == 0 || m_DrawWidth == 0)
	{
		if (m_DrawHeight == 0)
		{
			size.x = m_DrawWidth;
			auto factor = m_DrawWidth / originalSize.x;
			size.y = originalSize.y * factor;
		}
		else if (m_DrawWidth == 0)
		{
			size.y = m_DrawHeight;
			auto factor = m_DrawHeight / originalSize.y;
			size.x = originalSize.x * factor;
		}
	}
	else
	{
		size.x = m_DrawWidth;
		size.y = m_DrawHeight;
	}

	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, size.x, size.y);
}

void REC::SpriteRenderComponent::SetTexture(const std::string& textureName)
{
	// TODO: maybe use messenger system here. send request for resource and receive resource
	// prevent using resourcemanager directly in this class
	m_pTexture = ResourceManager::GetInstance().GetResource<Texture2D>(textureName);
}

void REC::SpriteRenderComponent::SetDrawSize(uint16_t width, uint16_t height)
{
	m_DrawWidth = width;
	m_DrawHeight = height;
}
