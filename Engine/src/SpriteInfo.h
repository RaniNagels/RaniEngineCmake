#pragma once
#include <string>
#include "../inc/GeneralStructs.h"

namespace REC
{
	// this struct contains all the data loaded in from a csv file
	struct SpriteInfo final
	{
		TextureRegion pixelRegion{};
		std::string key{};
	};
}