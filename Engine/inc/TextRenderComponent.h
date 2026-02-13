#pragma once
#include <string>
#include <memory>
#include "SpriteRenderComponent.h"
#include <SDL3/SDL_pixels.h>

namespace REC
{
	class Font;
	class Texture2D;

	class TextRenderComponent final : public SpriteRenderComponent
	{
	public:
		explicit TextRenderComponent(const std::string& text, Font* font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~TextRenderComponent();
	
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;
	
		void Update(float deltaT) override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	private:
		bool m_needsUpdate{};
		// TODO: move ownership to resourceManager. currently the component owns the texture, not ideal
		std::unique_ptr<Texture2D> m_textTexture{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		Font* m_font{};
	};
}