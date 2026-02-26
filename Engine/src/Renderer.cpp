#include <stdexcept>
#include <cstring>
#include <iostream>
#include "Renderer.h"
#include "../inc/SceneManager.h"
#include "../inc/Scene.h"
#include "Texture2D.h"
#include "SDLHelpers.h"

void REC::Renderer::Init(SDL_Window* window)
{
	m_window = window;

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
	m_renderer = SDL_CreateRenderer(window, nullptr);
#else
	m_Renderer = SDL_CreateRenderer(window, nullptr);
#endif

	if (m_Renderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void REC::Renderer::Render(Scene* scene) const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	scene->Render();

	SDL_RenderPresent(m_Renderer);
}

void REC::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void REC::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	RenderTexture(texture, x, y, dst.w, dst.h);
}

void REC::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void REC::Renderer::RenderTexture(const Texture2D& texture, const Rect& textureRegion, const Rect& screenRegion) const
{
	if (!(textureRegion.width == 0 && textureRegion.height == 0))
	{
		SDL_FRect src = ToRect(textureRegion);
		SDL_FRect dst = ToRect(screenRegion);
		SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
	}
	else
	{
		RenderTexture(texture, screenRegion.x, screenRegion.y, screenRegion.width, screenRegion.height);
	}
}

void REC::Renderer::RenderLine(const Color& color, glm::vec2 start, glm::vec2 end)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(m_Renderer, start.x, start.y, end.x, end.y);
}

void REC::Renderer::RenderRect(const Color& color, const Rect& rect, bool fill)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_FRect sdl_rect = ToRect(rect);
	if (fill)
		SDL_RenderFillRect(m_Renderer, &sdl_rect);
	else
		SDL_RenderRect(m_Renderer, &sdl_rect);
}

SDL_Renderer* REC::Renderer::GetSDLRenderer() const { return m_Renderer; }
