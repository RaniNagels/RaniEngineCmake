#pragma once
#include <cstdint>
#include <GameObject.h>
#include <EngineContext.h>

namespace REC
{
	// TODO: rename to IInputCommand
	// specifically designed for inputhandling
	// when needed for other cases => rename to IInputCommand, and create I(type)Command
	class ICommand
	{
	public:
		ICommand(const EngineContext& context): m_Context{context}{}
		virtual ~ICommand() = default;

		// controllerIndex => the controller that triggerd the command
		// inputStrength => the strength with witch it was triggered (thumb, triggers)
		virtual void Execute(uint8_t controllerIndex = -1, float inputStrength = 1.f) = 0;

	protected:
		const EngineContext* GetContext() const { return &m_Context; }

	private:
		EngineContext m_Context;
	};

	class GameObjectCommand : public ICommand
	{
	public:
		GameObjectCommand(const EngineContext& context, GameObject* actor) : ICommand{ context }, m_Actor { actor } {}
		virtual ~GameObjectCommand() = default;

	protected:
		GameObject* GetGameObject() const { return m_Actor; }

	private:
		GameObject* m_Actor;
	};
}