#pragma once
#include "Component.h"
#include "../RECColor.h"
#include <string>

namespace REC
{
	class Font;
	class TextRenderComponent;
	class FPSComponent final : public Component
	{
	public:
		explicit FPSComponent(GameObject* owner, Font* font, const Color& color = { 255, 255, 255, 255 });
		explicit FPSComponent(GameObject* owner, const std::string& font, const Color& color = { 255, 255, 255, 255 });
		virtual ~FPSComponent() = default;

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		virtual void Update(float deltaT) override;

	private:
		float m_AccumulatedTime{};
		float m_FPS{};
		TextRenderComponent* m_pTextRenderComponent;
	};
}