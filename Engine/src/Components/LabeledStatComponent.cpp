#include <Components/LabeledStatComponent.h>
#include <Components/TextRenderComponent.h>
#include <GameObject.h>

REC::LabeledStatComponent::LabeledStatComponent(GameObject* owner, const LabeledStatDescriptor& descriptor)
	: Component{owner}
	, m_Descriptor{descriptor}
	, m_StatValue{descriptor.initialValue}
{

	if (!GetOwner()->HasComponent<TextRenderComponent>())
		m_pTextRenderComponent = GetOwner()->AddComponent<REC::TextRenderComponent>(CreateText(), m_Descriptor.fontkey, m_Descriptor.color);
	else
	{
		m_pTextRenderComponent = GetOwner()->GetComponent<TextRenderComponent>();
		m_pTextRenderComponent->RequestFont(m_Descriptor.fontkey);
		m_pTextRenderComponent->SetText(CreateText());
		m_pTextRenderComponent->SetColor(m_Descriptor.color);
	}
}

void REC::LabeledStatComponent::Update(float)
{

}

std::string REC::LabeledStatComponent::CreateText()
{
	return m_Descriptor.label + ": " + std::to_string(m_StatValue);
}
