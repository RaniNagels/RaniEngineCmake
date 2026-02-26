#pragma once
#include <cstdint>

namespace REC
{
	// used to pass to the SpriteRenderComponent
	struct SpriteDescriptor final
	{
		uint16_t drawHeight = 0; // the desired height of the drawn sprite, not the actual height of the texture
		uint16_t drawWidth = 0; // the desired widht of the drawn sprite, not the actual width of the texture

		std::string dataResourceFile{}; // the file that stores the SpriteInfos
		std::string spriteDataKey{}; // the key used to store the specific SpriteInfo

		explicit SpriteDescriptor() = default;
		explicit SpriteDescriptor(int height, int width)
			: drawHeight{uint16_t(height)}, drawWidth{uint16_t(width)}
		{}
	};
}