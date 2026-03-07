#pragma once
#include <cstdint>
#include <GameObject.h>

namespace REC
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;

		// controllerIndex => the controller that triggerd the command
		// inputStrength => the strength with witch it was triggered (thumb, triggers)
		virtual void Execute(uint8_t controllerIndex = -1, float inputStrength = 1.f) = 0;
	};

	class GameObjectCommand : public ICommand
	{
	public:
		GameObjectCommand(GameObject* actor): m_Actor{actor} {}
		virtual ~GameObjectCommand() = default;
	protected:
		GameObject* GetGameObject() const { return m_Actor; }
	private:
		GameObject* m_Actor;
	};
}