#include "TimeSystem.h"

REC::TimeSystem::TimeSystem()
	: m_CurrentTime{ std::chrono::steady_clock::now() }
	, m_LastFrameTime{ m_CurrentTime } // m_StartTime must be initialized/declared before m_LastFrameTime!
	, m_DeltaTime{ 0.f }
	, m_MiliSecPerFrame{ 1000.f / 60.f } // default to 60 fps
{ }

void REC::TimeSystem::Update()
{
	m_CurrentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> delta = m_CurrentTime - m_LastFrameTime;

	m_LastFrameTime = m_CurrentTime;
	m_DeltaTime = delta.count();
}

std::chrono::nanoseconds REC::TimeSystem::GetSleepTime() const
{
	auto frameTime = std::chrono::high_resolution_clock::now() - m_LastFrameTime;
	auto targetFrameDuration = std::chrono::milliseconds(long(m_MiliSecPerFrame));
	
	// in case the frame took longer than the recerved amound of milliseconds per frame
	// do not sleep if all time has been spend already! 
	// avoid sleeping for a negative amount of time
	if (frameTime >= targetFrameDuration)
		return std::chrono::nanoseconds(0);

	auto sleeptime = targetFrameDuration - frameTime;

	return sleeptime;
}

void REC::TimeSystem::SetFrameRate(uint8_t frameRate)
{
	if (frameRate == 0)	
		frameRate = 1; // prevent division by zero!

	m_MiliSecPerFrame = 1000.f / frameRate;
}
