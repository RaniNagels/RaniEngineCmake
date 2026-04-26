#pragma once

namespace REC
{
	class SceneManager;
	class InputSystem;
	class IRenderer;
	class EventSystem;
	class IResourceManager;

	struct EngineContext
	{
		SceneManager* sceneManager;
		InputSystem* inputSystem;
		IRenderer* renderer;
		EventSystem* eventSystem;
		IResourceManager* resourceManager;
	};
}