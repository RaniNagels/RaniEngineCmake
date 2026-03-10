#pragma once
#include <Commands/ICommand.h>

// TODO: change game namespace
namespace Game
{
	class MoveCommand final : public REC::GameObjectInputCommand
	{
	public:
		explicit MoveCommand(REC::GameObject* actor, glm::vec2 direction, float speed);
		~MoveCommand() = default;

		virtual void Execute() override;

	private:
		glm::vec2 m_Direction;
		float m_Speed;
	};
}