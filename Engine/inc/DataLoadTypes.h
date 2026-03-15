#pragma once

namespace REC
{
	// the types that can be loaded from a dataFile
	enum class LoadTypes : uint8_t
	{
		Frames = 0x01,
		Animations = 0x02,
		TextureFont = 0x04,
	};

	inline LoadTypes operator|(LoadTypes a, LoadTypes b)
	{
		return static_cast<LoadTypes>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline LoadTypes operator&(LoadTypes a, LoadTypes b)
	{
		return static_cast<LoadTypes>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}
}