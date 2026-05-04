#pragma once
#include <Commands/ICommand.h>
#include <Events/Event.h>

#include <memory>

// TODO: change game namespace
namespace Game
{
	class GridComponent;

	struct MoveEventArgs : public REC::EventArgs
	{
		glm::vec2 direction{};
		float speed{};
		REC::GameObject* actor{ nullptr };

		virtual std::unique_ptr<REC::EventArgs> makeUnique() const override
		{
			return std::make_unique<MoveEventArgs>(*this);
		}
	};

	class MoveCommand final : public REC::GameObjectInputCommand
	{
	public:
		explicit MoveCommand(REC::GameObject* actor, glm::vec2 direction, float speed, GridComponent* playGroundGrid);
		~MoveCommand() = default;

		MoveCommand(const MoveCommand&) = delete;
		MoveCommand(MoveCommand&&) = delete;
		MoveCommand& operator= (const MoveCommand&) = delete;
		MoveCommand& operator= (const MoveCommand&&) = delete;

		virtual void Execute(float deltaTime) override;

	private:
		glm::vec2 m_Direction;
		float m_Speed;

		std::unique_ptr<REC::Event> m_pMoveEvent;

		GridComponent* m_pPlayGroundGrid{ nullptr };
	};
}