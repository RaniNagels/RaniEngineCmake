#pragma once
#include <string>

#include <GeneralStructs.h>

namespace REC
{
	// this struct contains all the data loaded in from a json file
	struct FrameInfo final
	{
		Rect pixelRegion{};
		std::string textureFileKey{};
	};
}