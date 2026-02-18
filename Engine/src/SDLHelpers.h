#pragma once
#include <SDL3/SDL.h>
#include "../inc/RECColor.h"

namespace REC
{
	inline SDL_Color ToSDL(const Color& color)
	{
		return SDL_Color{ color.r, color.g, color.b, color.a };
	}
}