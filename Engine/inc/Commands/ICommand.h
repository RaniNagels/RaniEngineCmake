#pragma once
#include <cstdint>
#include <algorithm>

#include <GameObject.h>
#include <EngineContext.h>

namespace REC
{
	// TODO: REFACTOR
	// TODO: rename to IInputCommand
	// specifically designed for inputhandling
	// when needed for other cases => rename to IInputCommand, and create I(type)Command
	class ICommand
	{
	public:
		ICommand() = default;
		virtual ~ICommand() = default;

		virtual void Execute(float deltaTime) = 0;
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
			, m_Strength{1.f}
		{ }
		virtual ~GameObjectInputCommand() = default;

		// inputStrength => the strength with witch it was triggered (thumb, triggers)
		virtual void SetStrength(float inputStrength = 1.f)
		{
			m_Strength = inputStrength;
		}

	protected:
		float GetStrength() const { return m_Strength; }

	private:
		float m_Strength;
	};
}