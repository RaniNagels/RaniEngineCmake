#include "../inc/TextRenderComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "../Renderer.h"
#include "../Font.h"
#include "../Texture2D.h"
#include <memory>
#include <stdexcept>
#include "../ResourceManager.h"

REC::TextRenderComponent::TextRenderComponent(const std::string& text, Font* font, const SDL_Color& color)
	: SpriteRenderComponent(nullptr)
	, m_NeedsUpdate{true}
	, m_TextTexture{}
	, m_Text{text}
	, m_Color{color}
	, m_Font{font}
{
}

REC::TextRenderComponent::TextRenderComponent(const std::string& text, const std::string& font, const SDL_Color& color)
	: SpriteRenderComponent(nullptr)
	, m_NeedsUpdate{true}
	, m_TextTexture{}
	, m_Text{text}
	, m_Color{color}
{
	SetFont(font);
}

REC::TextRenderComponent::~TextRenderComponent()
{
}

void REC::TextRenderComponent::Update(float)
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Text.length(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);

		m_TextTexture = std::make_unique<Texture2D>(texture);
		SetTexture(m_TextTexture.get());

		m_NeedsUpdate = false;
	}
}

void REC::TextRenderComponent::SetText(const std::string& text)
{
	if (text == m_Text) return;

	m_NeedsUpdate = true;
	m_Text = text;
}

void REC::TextRenderComponent::SetColor(const SDL_Color& color)
{
	if (m_Color.r == color.r && m_Color.g == color.g && m_Color.b == color.b && m_Color.a == color.a) return;

	m_NeedsUpdate = true;
	m_Color = color;
}

void REC::TextRenderComponent::SetFont(const std::string& font)
{
	// TODO: maybe use messenger system here. send request for resource and receive resource
	// prevent using resourcemanager directly in this class
	m_Font = ResourceManager::GetInstance().GetResource<Font>(font);
	m_NeedsUpdate = true;
}
