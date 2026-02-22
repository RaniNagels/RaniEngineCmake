#pragma once
#include <SDL3/SDL.h>
#include "../inc/RECColor.h"
#include "../inc/SpriteDescriptors.h"

namespace REC
{
	inline SDL_Color ToSDL(const Color& color)
	{
		return SDL_Color{ color.r, color.g, color.b, color.a };
	}

	inline SDL_FRect ToRect(const Rect& region)
	{
		return SDL_FRect{ region.x, region.y, region.width, region.height };
	}
}