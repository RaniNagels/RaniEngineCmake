#pragma once
#include <ICommand.h>

// TODO: change game namespace
namespace Game
{
	class MoveCommand final : public REC::GameObjectCommand
	{
	public:
		explicit MoveCommand(REC::GameObject* actor, glm::vec2 direction, float speed);
		~MoveCommand() = default;

		virtual void Execute(uint8_t controllerIndex = -1, float inputStrength = 1.f) override;

	private:
		glm::vec2 m_Direction;
		float m_Speed;
	};
}