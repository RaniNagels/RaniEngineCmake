#pragma once
#include <Commands/ICommand.h>

namespace REC
{
	class HealthComponent;
}

namespace Game
{
	// TODO: temporary class! normally health should change when player is hit by a bom or enemy
	class ChangeHealthCommand final : public REC::GameObjectInputCommand
	{
	public:
		explicit ChangeHealthCommand(REC::GameObject* actor, float changeDelta);
		~ChangeHealthCommand() = default;

		ChangeHealthCommand(const ChangeHealthCommand&) = delete;
		ChangeHealthCommand(ChangeHealthCommand&&) = delete;
		ChangeHealthCommand& operator= (const ChangeHealthCommand&) = delete;
		ChangeHealthCommand& operator= (const ChangeHealthCommand&&) = delete;

		virtual void Execute(float deltaTime) override;

	private:
		float m_HealthDelta;
		REC::HealthComponent* m_pHealthComponent;
	};
}