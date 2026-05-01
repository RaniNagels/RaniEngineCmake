#pragma once
#include <cstdint>
#include <string>
#include <memory>

#include <RECColor.h>
#include <GeneralStructs.h>

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

		float drawPointX{ 0.f }; // the percentagual x coordinate of the texture where the sprite shall be drawn from (0.f = left, 1.f = right)
		float drawPointY{ 0.f }; // the percentagual y coordinate of the texture where the sprite shall be drawn from (0.f = top, 1.f = bottom)

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

	enum class CollisionType : uint8_t
	{
		Static,
		Dynamic,
		NoCollision
	};

	struct CollisionDescriptor final : ComponentDescriptor
	{
		std::vector<Rect> bounds{}; // a vector allows for multiple collisionboxes per object allowing for more complex shapes
		CollisionType collisionType;
		// relative to the position of the gameobject, not absolute

		std::unique_ptr<ComponentDescriptor> Clone() const override
		{
			return std::make_unique<CollisionDescriptor>(*this);
		}
	};
}