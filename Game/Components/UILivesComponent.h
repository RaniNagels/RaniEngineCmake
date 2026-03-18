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
	class UILivesComponent final : public REC::LabeledStatComponent, public REC::IListener
	{
	public:
		explicit UILivesComponent(REC::GameObject* owner, const REC::LabeledStatDescriptor& descriptor);
		virtual ~UILivesComponent() = default;

		UILivesComponent(const UILivesComponent& other) = delete;
		UILivesComponent(UILivesComponent&& other) = delete;
		UILivesComponent& operator=(const UILivesComponent& other) = delete;
		UILivesComponent& operator=(UILivesComponent&& other) = delete;

		virtual void Notify(REC::Event* event) override;

	};
}