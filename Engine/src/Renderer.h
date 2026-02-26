#pragma once
#include <SDL3/SDL.h>
#include "../inc/RECColor.h"
#include "../inc/Singleton.h"
#include "../inc/SpriteDescriptor.h"
#include "../inc/GeneralStructs.h"

namespace REC
{
	class Texture2D;
	class Scene;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render(Scene* scene) const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, const Rect& textureRegion, const Rect& screenRegion) const;

		SDL_Renderer* GetSDLRenderer() const;

		const Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const Color& color) { m_clearColor = color; }

	private:
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		Color m_clearColor{};	
	};
}

