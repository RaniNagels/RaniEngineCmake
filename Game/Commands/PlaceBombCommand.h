#pragma once
#include <Commands/ICommand.h>

namespace REC
{
	class ValueChangedEvent;
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

		void SubscribeToEvents(REC::IListener* listener);
		void UnSubscribeToEvents(REC::IListener* listener);

	private:
		REC::SceneManager* m_pSceneManager; // retrieve active scene and add new GameObject to it
		std::unique_ptr<REC::ValueChangedEvent> m_HasPlacedBombEvent;
	};
}