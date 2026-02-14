#pragma once
#include "TextRenderComponent.h"

namespace REC
{
	class FPSRenderComponent final : public TextRenderComponent
	{
	public:
		explicit FPSRenderComponent(Font* font, const SDL_Color& color = { 255, 255, 255, 255 });
		explicit FPSRenderComponent(const std::string& font, const SDL_Color& color = { 255, 255, 255, 255 });
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