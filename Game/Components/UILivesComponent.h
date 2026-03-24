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

		void SetConnectedPlayer(REC::GameObject* player) { m_pConnectedPlayer = player; }

		virtual void Notify(REC::Event* event) override;

	private:
		REC::GameObject* m_pConnectedPlayer{};
	};
}