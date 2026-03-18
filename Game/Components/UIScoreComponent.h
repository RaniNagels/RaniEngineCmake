#pragma once
#include <Components/LabeledStatComponent.h>
#include <Components/IListener.h>

namespace REC
{
	class GameObject;
	class Event;
}

namespace Game
{
	class UIScoreComponent final : public REC::LabeledStatComponent, public REC::IListener
	{
	public:
		explicit UIScoreComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor);
		virtual ~UIScoreComponent() = default;

		UIScoreComponent(const UIScoreComponent& other) = delete;
		UIScoreComponent(UIScoreComponent&& other) = delete;
		UIScoreComponent& operator=(const UIScoreComponent& other) = delete;
		UIScoreComponent& operator=(UIScoreComponent&& other) = delete;

		virtual void Notify(REC::Event* event) override;

	};
}