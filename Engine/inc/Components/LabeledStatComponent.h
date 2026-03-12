#pragma once
#include <Components/Component.h>
#include <ComponentDescriptors.h>

namespace REC
{
	class TextRenderComponent;
	class LabeledStatComponent final : public Component
	{
	public:
		explicit LabeledStatComponent(GameObject* owner, const LabeledStatDescriptor& descriptor );
		virtual ~LabeledStatComponent() = default;

		LabeledStatComponent(const LabeledStatComponent& other) = delete;
		LabeledStatComponent(LabeledStatComponent&& other) = delete;
		LabeledStatComponent& operator=(const LabeledStatComponent& other) = delete;
		LabeledStatComponent& operator=(LabeledStatComponent&& other) = delete;

		virtual void Update(float deltaT) override;

	private:
		LabeledStatDescriptor m_Descriptor;
		int m_StatValue;
		TextRenderComponent* m_pTextRenderComponent;

		std::string CreateText();
	};
}