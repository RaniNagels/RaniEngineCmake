#pragma once
#include <Commands/ICommand.h>

namespace Game
{
	class MarkerComponent;
	class MoveMarkerCommand : public REC::GameObjectInputCommand
	{
	public:
		explicit MoveMarkerCommand(REC::GameObject* actor, bool moveUp);
		virtual ~MoveMarkerCommand() = default;

		MoveMarkerCommand(const MoveMarkerCommand&) = delete;
		MoveMarkerCommand(MoveMarkerCommand&&) noexcept = delete;
		MoveMarkerCommand& operator=(const MoveMarkerCommand&) = delete;
		MoveMarkerCommand& operator=(MoveMarkerCommand&&) noexcept = delete;

		virtual void Execute(float deltaTime) override;

	private:
		bool m_MoveUp;
		MarkerComponent* m_pMarkerComponent{ nullptr };
	};
}