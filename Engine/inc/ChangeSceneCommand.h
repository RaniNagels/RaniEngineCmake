#pragma once
#include <ICommand.h>
#include <Scene.h>
#include <SceneManager.h>

namespace REC
{
	class ChangeSceneCommand : public ICommand
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

		virtual void Execute(uint8_t controllerIndex = -1, float inputStrength = 1.f) override;
	private:
		Scene* m_FromScene;
		Scene* m_ToScene;
	};
}