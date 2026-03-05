#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include "EngineSettings.h"
#include "ResourceCreateInfos.h"
#include <Input/InputBinding.h>

namespace REC
{
	class TimeSystem;
	class Window;
	class SceneManager;
	class InputSystem;
	class IResourceManager;

	class Engine final
	{
	public:
		explicit Engine(const std::filesystem::path& dataPath);
		~Engine();

		void Run(const std::function<void(Engine*)>& load);
		void RunOneFrame();

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;

		void SetEngineData(const EngineSettings& data);
		SceneManager* GetSceneManager() const { return m_pSceneManager.get(); }
		InputSystem* GetInputSystem() const { return m_pInputSystem.get(); }

		void AddResources(const std::vector<ResourceCreateInfo*>& resources);
		InputBinding* CreateInputAction();

	private:
		std::unique_ptr<TimeSystem> m_pTimeSystem;
		std::unique_ptr<Window> m_pWindow;
		std::unique_ptr<SceneManager> m_pSceneManager;
		std::unique_ptr<InputSystem> m_pInputSystem;
	};
}