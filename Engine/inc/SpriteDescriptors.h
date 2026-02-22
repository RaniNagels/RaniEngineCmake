#pragma once
#include <cstdint>
#include "GeneralStructs.h"

namespace REC
{
	struct SpriteDescriptor final
	{
		uint16_t height = 0; // the desired height of the drawn sprite, not the actual height of the texture
		uint16_t width = 0; // the desired widht of the drawn sprite, not the actual width of the texture

		// draw region, which section of the texture is "cut out" and drawn on screen
		TextureRegion pixelRegion{};

		explicit SpriteDescriptor() = default;
		explicit SpriteDescriptor(int height, int width)
			: height{uint16_t(height)}, width{uint16_t(width)}
		{}
	};
}