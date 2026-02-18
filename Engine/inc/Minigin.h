#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include "EngineDescriptor.h"

namespace REC
{
	class TimeSystem;
	class Window;
	class SceneManager;
	class InputSystem;

	class Minigin final
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();

		void Run(const std::function<void(Minigin*)>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		void SetEngineData(const EngineDesc& data);
		SceneManager* GetSceneManager() const { return m_pSceneManager.get(); }
		InputSystem* GetInputSystem() const { return m_pInputSystem.get(); }

	private:
		std::unique_ptr<TimeSystem> m_pTimeSystem;
		std::unique_ptr<Window> m_pWindow;
		std::unique_ptr<SceneManager> m_pSceneManager;
		std::unique_ptr<InputSystem> m_pInputSystem;
	};
}