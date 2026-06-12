#pragma once
#include <Commands/ICommand.h>

namespace Game
{
	class ToggleSoundCommand : public REC::ICommand
	{
	public:
		explicit ToggleSoundCommand() = default;
		virtual ~ToggleSoundCommand() = default;

		virtual void Execute(float deltaTime) override;
	};
}