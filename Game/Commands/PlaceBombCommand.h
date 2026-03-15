#pragma once
#include <Commands/ICommand.h>

// TODO: change namespace
namespace Game
{
	class PlaceBombCommand final : public REC::GameObjectInputCommand
	{
	public:
		explicit PlaceBombCommand(REC::GameObject* actor, REC::SceneManager* sceneManager);
		~PlaceBombCommand() = default;

		PlaceBombCommand(const PlaceBombCommand&) = delete;
		PlaceBombCommand(PlaceBombCommand&&) = delete;
		PlaceBombCommand& operator= (const PlaceBombCommand&) = delete;
		PlaceBombCommand& operator= (const PlaceBombCommand&&) = delete;

		virtual void Execute(float deltaTime) override;

	private:
		REC::SceneManager* m_pSceneManager; // retrieve active scene and add new GameObject to it
	};
}