#include "../../inc/Components/SpriteRenderComponent.h"
#include "../../inc/Components/TransformComponent.h"
#include "../../inc/GameObject.h"
#include "../../inc/ResourceManager.h"
#include "../Renderer.h"

REC::SpriteRenderComponent::SpriteRenderComponent(GameObject* owner, Texture2D* texture, uint16_t width, uint16_t height)
	: RenderComponent(owner)
	, m_pTexture{texture}
	, m_Descriptor{}
{
	m_Descriptor.width = width;
	m_Descriptor.height = height;
}

REC::SpriteRenderComponent::SpriteRenderComponent(GameObject* owner, const std::string& textureName, uint16_t width, uint16_t height)
	: RenderComponent(owner)
	, m_Descriptor{}
{
	SetTexture(textureName);
	m_Descriptor.width = width;
	m_Descriptor.height = height;
}

REC::SpriteRenderComponent::SpriteRenderComponent(GameObject* owner, const std::string& textureName, const SpriteDescriptor& descriptor)
	: RenderComponent(owner)
	, m_Descriptor{descriptor}
{
	SetTexture(textureName);
}

REC::SpriteRenderComponent::SpriteRenderComponent(GameObject* owner, Texture2D* texture, const SpriteDescriptor& descriptor)
	: RenderComponent(owner)
	, m_pTexture{ texture }
	, m_Descriptor{descriptor}
{
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
	if (m_Descriptor.pixelRegion.IsValid())
	{
		src = m_Descriptor.pixelRegion;
		textureSize = {m_Descriptor.pixelRegion.width(), m_Descriptor.pixelRegion.height()};
	}
	else
		textureSize = m_pTexture->GetSize();

	if (m_Descriptor.height == 0 && m_Descriptor.width == 0)
	{
		dst.width = textureSize.x;
		dst.height = textureSize.y;
	}
	else if (m_Descriptor.height == 0 || m_Descriptor.width == 0)
	{
		if (m_Descriptor.height == 0)
		{
			dst.width = m_Descriptor.width;
			auto factor = m_Descriptor.width / textureSize.x;
			dst.height = textureSize.y * factor;
		}
		else if (m_Descriptor.width == 0)
		{
			dst.height = m_Descriptor.height;
			auto factor = m_Descriptor.height / textureSize.y;
			dst.width = textureSize.x * factor;
		}
	}
	else
	{
		dst.width = m_Descriptor.width;
		dst.height = m_Descriptor.height;
	}

	Renderer::GetInstance().RenderTexture(*m_pTexture, src, dst);
}

void REC::SpriteRenderComponent::SetTexture(const std::string& textureName)
{
	// TODO: maybe use messenger system here. send request for resource and receive resource
	// prevent using resourcemanager directly in this class
	m_pTexture = ResourceManager::GetInstance().GetResource<Texture2D>(textureName);
}
