#pragma once
#include <string>
#include "../inc/GeneralStructs.h"

namespace REC
{
	// this struct contains all the data loaded in from a csv file
	struct FrameInfo final
	{
		Rect pixelRegion{};
		std::string key{};
		std::string textureFileKey{};
	};
}