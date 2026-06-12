#pragma once
#include <Components/LabeledStatComponent.h>
#include <Components/IListener.h>

namespace Game
{
	class UICurrentLevelComponent final : public REC::LabeledStatComponent, public REC::IListener
	{
	public:
		explicit UICurrentLevelComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor);
		virtual ~UICurrentLevelComponent() = default;

		UICurrentLevelComponent(const UICurrentLevelComponent& other) = delete;
		UICurrentLevelComponent(UICurrentLevelComponent&& other) = delete;
		UICurrentLevelComponent& operator=(const UICurrentLevelComponent& other) = delete;
		UICurrentLevelComponent& operator=(UICurrentLevelComponent&& other) = delete;

		virtual void Notify(REC::Event* event) override;
		virtual void Destroy() override;
	};
}