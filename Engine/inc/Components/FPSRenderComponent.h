#pragma once
#include "TextRenderComponent.h"

namespace REC
{
	class FPSRenderComponent final : public TextRenderComponent
	{
	public:
		explicit FPSRenderComponent(GameObject* owner, Font* font, const Color& color = { 255, 255, 255, 255 });
		explicit FPSRenderComponent(GameObject* owner, const std::string& font, const Color& color = { 255, 255, 255, 255 });
		virtual ~FPSRenderComponent() = default;

		FPSRenderComponent(const FPSRenderComponent& other) = delete;
		FPSRenderComponent(FPSRenderComponent&& other) = delete;
		FPSRenderComponent& operator=(const FPSRenderComponent& other) = delete;
		FPSRenderComponent& operator=(FPSRenderComponent&& other) = delete;

		virtual void Update(float deltaT) override;

	private:
		float m_AccumulatedTime{};
	};
}