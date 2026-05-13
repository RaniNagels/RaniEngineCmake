#pragma once
#include <string>
#include <functional>
#include <filesystem>

#include <Resources/ResourceCreateInfos.h>
#include <Input/InputBinding.h>
#include <EngineContext.h>
#include <IEngine.h>

namespace REC
{
	class TimeSystem;
	class Window;
	class SceneManager;
	class InputSystem;
	class IResourceManager;
	class IRenderer;
	class EventSystem;
	class CollisionSystem;
	class PhysicsSystem;

	class Engine final : public IEngine
	{
	public:
		explicit Engine(const std::filesystem::path& dataPath);
		~Engine();

		virtual void Run(void(*load)(IEngine*)) override;
		void RunOneFrame(); // must be public for emscripten

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;

		virtual const EngineContext& GetContext() const override { return m_EngineContext; }
		virtual void SetEngineSettings(const EngineSettings& data) override;

	private:
		std::unique_ptr<TimeSystem> m_pTimeSystem;
		std::unique_ptr<Window> m_pWindow;
		std::unique_ptr<SceneManager> m_pSceneManager;
		std::unique_ptr<InputSystem> m_pInputSystem;
		std::unique_ptr<EventSystem> m_pEventSystem;
		std::unique_ptr<CollisionSystem> m_pCollisionSystem;
		std::unique_ptr<PhysicsSystem> m_pPhysicsSystem;

		EngineContext m_EngineContext{};
	};
}