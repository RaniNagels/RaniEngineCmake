#pragma once
#include <cstdint>
#include <GameObject.h>
#include <EngineContext.h>
#include <algorithm>

namespace REC
{
	// TODO: rename to IInputCommand
	// specifically designed for inputhandling
	// when needed for other cases => rename to IInputCommand, and create I(type)Command
	class ICommand
	{
	public:
		ICommand() = default;
		virtual ~ICommand() = default;

		virtual void Execute() = 0;
	};

	class EngineCommand : public ICommand
	{
	public:
		EngineCommand(const EngineContext& context)
			: ICommand{} 
			, m_Context{context}
		{ }
		virtual ~EngineCommand() = default;

	protected:
		const EngineContext* GetContext() const { return &m_Context; }

	private:
		EngineContext m_Context;

	};

	class GameObjectCommand : public ICommand
	{
	public:
		GameObjectCommand(GameObject* actor) : ICommand{ }, m_Actor { actor } {}
		virtual ~GameObjectCommand() = default;

	protected:
		GameObject* GetGameObject() const { return m_Actor; }

	private:
		GameObject* m_Actor;
	};

	class GameObjectInputCommand : public GameObjectCommand
	{
	public:
		GameObjectInputCommand(GameObject* actor)
			: GameObjectCommand{actor}
			, m_ControllerIndex{ static_cast<uint8_t>(-1)}
			, m_InputStrength{1.f}
		{ }
		virtual ~GameObjectInputCommand() = default;

		// controllerIndex => the controller that triggerd the command
		virtual void TriggeredByController(uint8_t controllerIndex = -1) { m_ControllerIndex = controllerIndex; }
		// inputStrength => the strength with witch it was triggered (thumb, triggers)
		virtual void SetInputStrength(float inputStrength = 1.f)
		{
			m_InputStrength = inputStrength;
		}

	protected:
		uint8_t GetControllerIndex() const { return m_ControllerIndex; }
		float GetInputStrength() const { return m_InputStrength; }

	private:
		uint8_t m_ControllerIndex;
		float m_InputStrength;
	};
}