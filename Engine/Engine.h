#pragma once
#include <string>
#include <functional>
#include <filesystem>

//#include "src/TimeSystem.h"

namespace REC
{
	class TimeSystem;
	class Window;

	class Engine final
	{
	public:
		explicit Engine(const std::filesystem::path& dataPath);
		~Engine();

		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;

	private:
		std::unique_ptr<TimeSystem> m_pTimeSystem;
		std::unique_ptr<Window> m_pWindow;
		bool m_quit{};
	};
}