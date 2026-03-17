#pragma once
#include <Commands/ICommand.h>

namespace REC
{
	class ControllerComponent;
}

// TODO: change game namespace
namespace Game
{
	class MoveCommand final : public REC::GameObjectInputCommand
	{
	public:
		explicit MoveCommand(REC::GameObject* actor, glm::vec2 direction, float speed);
		~MoveCommand() = default;

		MoveCommand(const MoveCommand&) = delete;
		MoveCommand(MoveCommand&&) = delete;
		MoveCommand& operator= (const MoveCommand&) = delete;
		MoveCommand& operator= (const MoveCommand&&) = delete;

		virtual void Execute(float deltaTime) override;

	private:
		glm::vec2 m_Direction;
		float m_Speed;
		REC::ControllerComponent* m_pControllerComponent;
	};
}