#include "../../inc/Components/SpriteRenderComponent.h"
#include "../../inc/Components/TransformComponent.h"
#include "../../inc/GameObject.h"
#include "../../inc/ResourceManager.h"
#include "../Renderer.h"
#include "../SpriteInfo.h"

REC::SpriteRenderComponent::SpriteRenderComponent(GameObject* owner, Texture2D* texture, uint16_t width, uint16_t height)
	: RenderComponent(owner)
	, m_pTexture{texture}
	, m_Descriptor{}
{
	m_Descriptor.drawWidth = width;
	m_Descriptor.drawHeight = height;
	m_pSpriteInfo = nullptr;
}

REC::SpriteRenderComponent::SpriteRenderComponent(GameObject* owner, const std::string& textureName, uint16_t width, uint16_t height)
	: RenderComponent(owner)
	, m_Descriptor{}
{
	SetTexture(textureName);
	m_Descriptor.drawWidth = width;
	m_Descriptor.drawHeight = height;
	m_pSpriteInfo = nullptr;
}

REC::SpriteRenderComponent::SpriteRenderComponent(GameObject* owner, const std::string& textureName, const SpriteDescriptor& descriptor)
	: RenderComponent(owner)
	, m_Descriptor{descriptor}
{
	SetTexture(textureName);
	SetSpriteInfo();
}

REC::SpriteRenderComponent::SpriteRenderComponent(GameObject* owner, Texture2D* texture, const SpriteDescriptor& descriptor)
	: RenderComponent(owner)
	, m_pTexture{ texture }
	, m_Descriptor{descriptor}
{
	SetSpriteInfo();
}

void REC::SpriteRenderComponent::Update(float)
{
}

void REC::SpriteRenderComponent::Render()
{
	auto* transform = this->GetOwner()->GetTransform();

	Rect src{};
	Rect dst{};

	if (transform)
	{
		glm::vec2 pos = transform->GetWorldPosition();
		dst.x = pos.x;
		dst.y = pos.y;
	}

	glm::vec2 textureSize{};
	if (m_pSpriteInfo != nullptr)
	{
		if (m_pSpriteInfo->pixelRegion.IsValid())
		{
			src = m_pSpriteInfo->pixelRegion;
			textureSize = { m_pSpriteInfo->pixelRegion.width(), m_pSpriteInfo->pixelRegion.height()};
		}
	}
	else
		textureSize = m_pTexture->GetSize();

	if (m_Descriptor.drawHeight == 0 && m_Descriptor.drawWidth == 0)
	{
		dst.width = textureSize.x;
		dst.height = textureSize.y;
	}
	else if (m_Descriptor.drawHeight == 0 || m_Descriptor.drawWidth == 0)
	{
		if (m_Descriptor.drawHeight == 0)
		{
			dst.width = m_Descriptor.drawWidth;
			auto factor = m_Descriptor.drawWidth / textureSize.x;
			dst.height = textureSize.y * factor;
		}
		else if (m_Descriptor.drawWidth == 0)
		{
			dst.height = m_Descriptor.drawHeight;
			auto factor = m_Descriptor.drawHeight / textureSize.y;
			dst.width = textureSize.x * factor;
		}
	}
	else
	{
		dst.width = m_Descriptor.drawWidth;
		dst.height = m_Descriptor.drawHeight;
	}

	Renderer::GetInstance().RenderTexture(*m_pTexture, src, dst);
}

void REC::SpriteRenderComponent::SetTexture(const std::string& textureName)
{
	// TODO: maybe use messenger system here. send request for resource and receive resource
	// prevent using resourcemanager directly in this class
	m_pTexture = ResourceManager::GetInstance().GetResource<Texture2D>(textureName);
}

void REC::SpriteRenderComponent::SetSpriteInfo()
{
	m_pSpriteInfo = ResourceManager::GetInstance().GetResource<SpriteInfo>(m_Descriptor.dataResourceFile, m_Descriptor.spriteDataKey);
}
