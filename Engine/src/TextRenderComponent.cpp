#include "../inc/TextRenderComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "../Renderer.h"
#include "../Font.h"
#include "../Texture2D.h"
#include <memory>
#include <stdexcept>

REC::TextRenderComponent::TextRenderComponent(const std::string& text, Font* font, const SDL_Color& color)
	: SpriteRenderComponent(nullptr)
	, m_needsUpdate{ true }
	, m_textTexture{}
	, m_text{ text }
	, m_color{ color }
	, m_font{ font }
{
}

REC::TextRenderComponent::~TextRenderComponent()
{
}

void REC::TextRenderComponent::Update(float)
{
	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
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

		m_textTexture = std::make_unique<Texture2D>(texture);
		SetTexture(m_textTexture.get());

		m_needsUpdate = false;
	}
}

void REC::TextRenderComponent::SetText(const std::string& text)
{
	if (text == m_text) return;

	m_needsUpdate = true;
	m_text = text;
}

void REC::TextRenderComponent::SetColor(const SDL_Color& color)
{
	if (m_color.r == color.r && m_color.g == color.g && m_color.b == color.b && m_color.a == color.a) return;

	m_needsUpdate = true;
	m_color = color;
}
