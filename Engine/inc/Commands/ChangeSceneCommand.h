#pragma once
#include <Commands/ICommand.h>
#include <Scene.h>
#include <SceneManager.h>

namespace REC
{
	class ChangeSceneCommand : public EngineCommand
	{
	public:
		// if fromScene is nullptr => from any scene to new one
		// else from this specific scene only to new scene
		ChangeSceneCommand(const EngineContext& context, Scene* toScene, Scene* fromScene = nullptr);
		~ChangeSceneCommand() = default;

		ChangeSceneCommand(const ChangeSceneCommand&) = delete;
		ChangeSceneCommand(ChangeSceneCommand&&) = delete;
		ChangeSceneCommand& operator= (const ChangeSceneCommand&) = delete;
		ChangeSceneCommand& operator= (const ChangeSceneCommand&&) = delete;

		virtual void Execute(float deltaTime) override;
	private:
		Scene* m_ToScene;
		Scene* m_FromScene;
	};
}