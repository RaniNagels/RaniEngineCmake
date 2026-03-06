#pragma once
#include <cstdint>
#include <GameObject.h>

namespace REC
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		virtual void Execute(uint8_t playerIndex = -1) = 0;
	};

	class GameActorCommand : public ICommand
	{
	public:
		GameActorCommand(GameObject* actor): m_Actor{actor} {}
		virtual ~GameActorCommand() = default;
	private:
		GameObject* m_Actor;
	};
}