#include "TimeSystem.h"
#include "TimeSystem.h"

REC::TimeSystem::TimeSystem()
	: m_StartTime{ std::chrono::steady_clock::now() }
	, m_LastFrameTime{ m_StartTime } // m_StartTime must be initialized/declared before m_LastFrameTime!
	, m_DeltaTime{ 0.f }
	, m_MiliSecPerFrame{ 1000.f / 60.f } // default to 60 fps
{ }

void REC::TimeSystem::Update()
{
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> delta = now - m_LastFrameTime;

	m_LastFrameTime = now;
	m_DeltaTime = delta.count();
}

std::chrono::nanoseconds REC::TimeSystem::GetSleepTime() const
{
	auto frameDuration = std::chrono::duration<float, std::milli>(m_MiliSecPerFrame);
	auto sleeptime = m_LastFrameTime + frameDuration - std::chrono::high_resolution_clock::now();

	// in case the frame took longer than the recerved amound of milliseconds per frame
	// do not sleep if all time has been spend already! 
	// avoid sleeping for a negative amount of time
	if (sleeptime < std::chrono::nanoseconds(0))
		return std::chrono::nanoseconds(0);

	return std::chrono::duration_cast<std::chrono::nanoseconds>(sleeptime);
}

void REC::TimeSystem::SetFrameRate(uint8_t frameRate)
{
	if (frameRate == 0)	
		frameRate = 1; // prevent division by zero!

	m_MiliSecPerFrame = 1000.f / frameRate;
}
