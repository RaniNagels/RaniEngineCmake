#pragma once
#include <cstdint>
#include <string>
#include <RECColor.h>
#include <memory>

namespace REC
{
	struct ComponentDescriptor
	{
		virtual ~ComponentDescriptor() = default;
		virtual std::unique_ptr<ComponentDescriptor> Clone() const = 0;
	};

	// used to pass to the SpriteRenderComponent
	struct SpriteDescriptor final : ComponentDescriptor
	{
		uint16_t drawHeight = 0; // the desired height of the drawn sprite, not the actual height of the texture
		uint16_t drawWidth = 0; // the desired widht of the drawn sprite, not the actual width of the texture

		std::string frameDataFileKey{}; // the key used to store the file containing the frame Data
		std::string frameKey{}; // the key used to store the specific FrameInfo
		std::string textureKey{};

		explicit SpriteDescriptor() = default;
		explicit SpriteDescriptor(int height, int width)
			: drawHeight{uint16_t(height)}, drawWidth{uint16_t(width)}
		{}

		std::unique_ptr<ComponentDescriptor> Clone() const override
		{
			return std::make_unique<SpriteDescriptor>(*this);
		}
	};

	struct AnimationDescriptor final : ComponentDescriptor
	{
		// this class will overwrite the spriteddata key of the SpriteDescriptor
		std::string animationDataFileKey{};
		std::string animationKey{};
		bool startOnStartup{ true };

		std::unique_ptr<ComponentDescriptor> Clone() const override
		{
			return std::make_unique<AnimationDescriptor>(*this);
		}
	};

	// TODO: not yet in use
	// describes how text shall be printed on screen
	struct TextDescriptor final : ComponentDescriptor
	{
		bool isTextureFont{ false };
		std::string fontKey{}; // either Font or TextureFont key
		std::string text{}; // the text that needs to be printed
		Color color{0,0,0}; // will be ignored if key belongs to TextureFont

		std::unique_ptr<ComponentDescriptor> Clone() const override
		{
			return std::make_unique<TextDescriptor>(*this);
		}
	};

	struct LabeledStatDescriptor final : ComponentDescriptor
	{
		std::string fontkey{};
		std::string label{};
		int initialValue{};
		Color color{ 0,0,0 };

		std::unique_ptr<ComponentDescriptor> Clone() const override
		{
			return std::make_unique<LabeledStatDescriptor>(*this);
		}
	};
}