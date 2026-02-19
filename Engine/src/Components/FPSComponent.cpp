#include "../../inc/Components/FPSComponent.h"
#include "../../inc/Components/TextRenderComponent.h"
#include "../../inc/GameObject.h"
#include "../Font.h"
#include <chrono>
#include <iomanip>

REC::FPSComponent::FPSComponent(GameObject* owner, Font* font, const Color& color)
	: Component(owner)
{
	m_pTextRenderComponent = GetOwner()->AddComponent<REC::TextRenderComponent>("00.0 FPS", font, color);
}

REC::FPSComponent::FPSComponent(GameObject* owner, const std::string& font, const Color& color)
	: Component(owner)
{
	m_pTextRenderComponent = GetOwner()->AddComponent<REC::TextRenderComponent>("00.0 FPS", font, color);
}

void REC::FPSComponent::Update(float deltaT)
{
	m_AccumulatedTime += deltaT;
	float updateRate = 0.1f;
	
	if (m_AccumulatedTime >= updateRate)
	{
		auto fps = std::chrono::duration<float, std::milli>(1 / deltaT);
		if (fps.count() == m_FPS) return;
		m_FPS = fps.count();
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(1) << fps.count();
		m_pTextRenderComponent->SetText(ss.str() + "FPS");

		m_AccumulatedTime -= updateRate;
	}
}
