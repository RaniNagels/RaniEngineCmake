#include "../inc/FPSRenderComponent.h"
#include <chrono>
#include <iomanip>

REC::FPSRenderComponent::FPSRenderComponent(Font* font, const Color& color)
	: TextRenderComponent("00.0 FPS", font, color)
{
}

REC::FPSRenderComponent::FPSRenderComponent(const std::string& font, const Color& color)
	: TextRenderComponent("00.0 FPS", font, color)
{
}

void REC::FPSRenderComponent::Update(float deltaT)
{
	// !! do not forget to call the baseclass when overriding
	// otherwise no texture is created at all -> error!
	TextRenderComponent::Update(deltaT);

	m_AccumulatedTime += deltaT;
	float updateRate = 0.1f;
	
	if (m_AccumulatedTime >= updateRate)
	{
		auto fps = std::chrono::duration<float, std::milli>(1 / deltaT);
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(1) << fps.count();
		SetText(ss.str() + " FPS");

		m_AccumulatedTime -= updateRate;
	}

}
