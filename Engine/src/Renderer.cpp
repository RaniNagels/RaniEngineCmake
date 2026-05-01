#include "Renderer.h"

#include <stdexcept>
#include <cstring>
#include <iostream>

#include <SceneManager.h>
#include <Scene.h>
#include "Resources/ResourceTypes/Texture2D.h"
#include "SDLHelpers.h"

void REC::Renderer::Init(SDL_Window* window)
{
	m_pWindow = window;
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	m_pRenderer = SDL_CreateRenderer(window, nullptr);
	if (m_pRenderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
}

void REC::Renderer::Render(Scene* scene) const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_pRenderer);

	scene->Render(this);

	SDL_RenderPresent(m_pRenderer);
}

void REC::Renderer::Destroy()
{
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
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
	SDL_SetTextureScaleMode(texture.GetSDLTexture(), SDL_SCALEMODE_NEAREST);
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void REC::Renderer::RenderTexture(const Texture2D& texture, const Rect& textureRegion, const Rect& screenRegion) const
{
	if (!(textureRegion.width == 0 && textureRegion.height == 0))
	{
		SDL_FRect src = ToRect(textureRegion);
		SDL_FRect dst = ToRect(screenRegion);
		SDL_SetTextureScaleMode(texture.GetSDLTexture(), SDL_SCALEMODE_NEAREST);
		SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
	}
	else
	{
		RenderTexture(texture, screenRegion.x, screenRegion.y, screenRegion.width, screenRegion.height);
	}
}

void REC::Renderer::RenderLine(const Color& color, glm::vec2 start, glm::vec2 end) const
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(m_pRenderer, start.x, start.y, end.x, end.y);
}

void REC::Renderer::RenderRect(const Color& color, const Rect& rect, bool fill) const
{
	SDL_FRect sdl_rect = ToRect(rect);
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	if (fill)
	{
		SDL_RenderFillRect(m_pRenderer, &sdl_rect);
	}
	else
	{
		SDL_RenderRect(m_pRenderer, &sdl_rect);
	}
}

// Source - https://stackoverflow.com/a/74745126
// Posted by JanSordid, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-23, License - CC BY-SA 4.0
// minor modifations by Rani Nagels to prevent errors and use SDL3 instead of SDL2
void REC::Renderer::RenderCircle(const Color& color, glm::vec2 center, float radius) const
{
	// 35 / 49 is a slightly biased approximation of 1/sqrt(2)
	//const int arrSize = ((radius * 8 * 35 / 49) + (8 - 1)) & -8;
	const int arrSize = (static_cast<int>(radius * 8.0f * 35.0f / 49.0f) + 7) & ~7;
	std::vector<SDL_FPoint> points(arrSize);
	int drawCount = 0;

	const int32_t diameter = static_cast<int32_t>(radius * 2);

	int32_t x = static_cast<int32_t>(radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		// Each of the following renders an octant of the circle
		points[drawCount + 0] = { center.x + x, center.y - y };
		points[drawCount + 1] = { center.x + x, center.y + y };
		points[drawCount + 2] = { center.x - x, center.y - y };
		points[drawCount + 3] = { center.x - x, center.y + y };
		points[drawCount + 4] = { center.x + y, center.y - x };
		points[drawCount + 5] = { center.x + y, center.y + x };
		points[drawCount + 6] = { center.x - y, center.y - x };
		points[drawCount + 7] = { center.x - y, center.y + x };

		drawCount += 8;

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}

	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderPoints(m_pRenderer, points.data(), drawCount);
}

SDL_Renderer* REC::Renderer::GetSDLRenderer() const { return m_pRenderer; }
