#pragma once
#include <string>
#include <functional>
#include <filesystem>

//#include "src/TimeSystem.h"

namespace REC
{
	class TimeSystem;

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
		bool m_quit{};
	};
}