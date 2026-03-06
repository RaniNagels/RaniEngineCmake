#pragma once
#include <Components/Component.h>
#include <vector>
#include <ComponentDescriptors.h>

namespace REC
{
	class Texture2D;
	struct AnimationInfo;
	struct FrameInfo;
	class SpriteRenderComponent;

	// not a renderComponent, but will use a renderComponent!
	class AnimatedSpriteComponent final : public Component
	{
	public:
		explicit AnimatedSpriteComponent(GameObject* owner, const AnimationDescriptor& descriptor);
		virtual ~AnimatedSpriteComponent() = default;

		AnimatedSpriteComponent(const AnimatedSpriteComponent& other) = delete;
		AnimatedSpriteComponent(AnimatedSpriteComponent&& other) = delete;
		AnimatedSpriteComponent& operator=(const AnimatedSpriteComponent& other) = delete;
		AnimatedSpriteComponent& operator=(AnimatedSpriteComponent&& other) = delete;

		virtual void Update(float deltaT) override;

		void StartAnimation();
		void StopAnimation();
		bool IsAnimating();

	private:
		AnimationDescriptor m_Descriptor;
		std::vector<FrameInfo*> m_Frames{};
		AnimationInfo* m_AnimationInfo{};
		SpriteRenderComponent* m_pSpriteRenderComponent{ nullptr };
		float m_Counter{};
		uint8_t m_CurrentFrame{ 0 };
		bool m_Animating{ false };
		float m_FrameDuration{  };
	};
}