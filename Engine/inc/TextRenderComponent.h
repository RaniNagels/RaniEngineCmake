#pragma once
#include <string>
#include <memory>
#include "SpriteRenderComponent.h"
#include <SDL3/SDL_pixels.h>
#include "../Texture2D.h"

namespace REC
{
	class Font;
	class Texture2D;

	class TextRenderComponent : public SpriteRenderComponent
	{
	public:
		explicit TextRenderComponent(const std::string& text, Font* font, const SDL_Color& color = { 255, 255, 255, 255 });
		explicit TextRenderComponent(const std::string& text, const std::string& font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~TextRenderComponent() = default;
	
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;
	
		void Update(float deltaT) override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);
		void SetFont(const std::string& font);

	private:
		bool m_NeedsUpdate{};
		// TODO: move ownership to resourceManager. currently the component owns the texture, not ideal
		std::unique_ptr<Texture2D> m_TextTexture{};
		std::string m_Text{};
		SDL_Color m_Color{ 255, 255, 255, 255 };
		Font* m_Font{};
	};
}