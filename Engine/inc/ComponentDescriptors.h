#pragma once
#include <cstdint>
#include <string>
#include <RECColor.h>

namespace REC
{
	// used to pass to the SpriteRenderComponent
	struct SpriteDescriptor final
	{
		uint16_t drawHeight = 0; // the desired height of the drawn sprite, not the actual height of the texture
		uint16_t drawWidth = 0; // the desired widht of the drawn sprite, not the actual width of the texture

		std::string frameKey{}; // the key used to store the specific FrameInfo
		std::string textureKey{};

		explicit SpriteDescriptor() = default;
		explicit SpriteDescriptor(int height, int width)
			: drawHeight{uint16_t(height)}, drawWidth{uint16_t(width)}
		{}
	};

	struct AnimationDescriptor final
	{
		// this class will overwrite the spriteddata key of the SpriteDescriptor
		std::string animationKey{};
		bool startOnStartup{ true };
	};

	// TODO: not yet in use
	// describes how text shall be printed on screen
	struct TextDescriptor final
	{
		bool isTextureFont{ false };
		std::string fontKey{}; // either Font or TextureFont key
		std::string text{}; // the text that needs to be printed
		Color color{0,0,0}; // will be ignored if key belongs to TextureFont
	};
}