#pragma once
#include <cstdint>

namespace Game
{
	enum class RenderLayer : uint8_t
	{
		BACKGROUND = 0,
		UI = 1,
		PLACEBLES = 2,
		ENEMIES = 3,
		PLAYER = 4,
	};

	static uint8_t GetLayer(RenderLayer layer)
	{
		return static_cast<uint8_t>(layer);
	}
}