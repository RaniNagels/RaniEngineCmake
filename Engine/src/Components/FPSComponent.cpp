#include <Components/FPSComponent.h>

#include <chrono>
#include <iomanip>

#include <Components/TextRenderComponent.h>
#include <GameObject.h>

#include "Resources/ResourceManager.h"
#include "Resources/ResourceTypes/Font.h"

REC::FPSComponent::FPSComponent(GameObject* owner, Font* font, const Color& color)
	: Component(owner)
{
	if (font == nullptr)
		assert(false && "font cannot be nullptr");

	if (!GetOwner()->HasComponent<TextRenderComponent>())
		m_pTextRenderComponent = GetOwner()->AddComponent<REC::TextRenderComponent>("00.0 FPS", font, color);
	else
	{
		m_pTextRenderComponent = GetOwner()->GetComponent<TextRenderComponent>();
		m_pTextRenderComponent->SetFont(font);
		m_pTextRenderComponent->SetText("00.0 FPS");
		m_pTextRenderComponent->SetColor(color);
	}
}

REC::FPSComponent::FPSComponent(GameObject* owner, const std::string& font, const Color& color)
	: FPSComponent(owner, ResourceManager::GetInstance().GetResource<Font>(font), color)
{ }

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
