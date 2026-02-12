#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextObject::TextObject(const std::string& text, Font* font, const SDL_Color& color)
	: m_needsUpdate(true), m_text(text), m_color(color), m_font(font), m_textTexture(nullptr)
{ }

dae::TextObject::~TextObject()
{
	delete m_textTexture;
	m_textTexture = nullptr;
}

void dae::TextObject::Update()
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

		delete m_textTexture;
		m_textTexture = nullptr;

		m_textTexture = new Texture2D(texture);
		m_needsUpdate = false;
	}
}

void dae::TextObject::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

void dae::TextObject::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextObject::SetPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y);
}

void dae::TextObject::SetColor(const SDL_Color& color) 
{ 
	m_color = color; 
	m_needsUpdate = true; 
}


