#pragma once
#include <Commands/ICommand.h>

namespace REC
{
	class Event;
	class IListener;
}

// TODO: change namespace
namespace Game
{
	class PlaceBombCommand final : public REC::GameObjectInputCommand
	{
	public:
		explicit PlaceBombCommand(REC::GameObject* actor, REC::SceneManager* sceneManager);
		~PlaceBombCommand();

		PlaceBombCommand(const PlaceBombCommand&) = delete;
		PlaceBombCommand(PlaceBombCommand&&) = delete;
		PlaceBombCommand& operator= (const PlaceBombCommand&) = delete;
		PlaceBombCommand& operator= (const PlaceBombCommand&&) = delete;

		virtual void Execute(float deltaTime) override;

	private:
		REC::SceneManager* m_pSceneManager; // retrieve active scene and add new GameObject to it
		std::unique_ptr<REC::Event> m_HasPlacedBombEvent;
	};
}