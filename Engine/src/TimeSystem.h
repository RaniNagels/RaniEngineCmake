#pragma once
#include <chrono>

namespace REC
{
	class TimeSystem final
	{
	public:
		explicit TimeSystem();
		~TimeSystem() = default;

		TimeSystem(const TimeSystem& other) = delete;
		TimeSystem(TimeSystem&& other) = delete;
		TimeSystem& operator=(const TimeSystem& other) = delete;
		TimeSystem& operator=(TimeSystem&& other) = delete;

		void Init();
		void Update();

		float GetDeltaTime() const { return m_DeltaTime; }
		std::chrono::nanoseconds GetSleepTime() const;

		void SetFrameRate(uint8_t frameRate);

	private:

		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_LastFrameTime;

		float m_DeltaTime;
		float m_MiliSecPerFrame;
	};
}