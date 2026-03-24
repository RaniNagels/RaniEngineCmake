#pragma once
#include <cstdint>

namespace Game
{
	enum class RenderLayer : uint8_t
	{
		Background = 0,
		Ui = 1,
		Placables = 2,
		Enemies = 3,
		Player = 4,
	};

	static uint8_t GetLayer(RenderLayer layer)
	{
		return static_cast<uint8_t>(layer);
	}
}