#pragma once
#include <string>
#include <functional>
#include <filesystem>

//#include "src/TimeSystem.h"

namespace REC
{
	class TimeSystem;
	class Window;

	class Minigin final
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();

		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		std::unique_ptr<TimeSystem> m_pTimeSystem;
		std::unique_ptr<Window> m_pWindow;
		bool m_quit{};
	};
}