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

	struct TextDescriptor final : ComponentDescriptor
	{
		std::string fontKey{}; 
		std::string text{}; // the text that needs to be printed
		Color color{0,0,0};

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