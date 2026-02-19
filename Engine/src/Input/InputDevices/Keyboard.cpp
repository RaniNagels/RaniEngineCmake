#include "Keyboard.h"

void REC::Keyboard::ResetState()
{
	m_Previous = m_Current;
}

void REC::Keyboard::UpdateState(uint8_t key, bool isDown)
{
	m_Current[key] = isDown;
}

bool REC::Keyboard::IsPressed(Input::Keyboard::Button key) const
{
	auto k = static_cast<uint8_t>(key);
	return m_Current[k];
}

bool REC::Keyboard::IsDownThisFrame(Input::Keyboard::Button key) const
{
	auto k = static_cast<uint8_t>(key);
	return m_Previous[k] == false && m_Current[k] == true;
}

bool REC::Keyboard::IsUpThisFrame(Input::Keyboard::Button key) const
{
	auto k = static_cast<uint8_t>(key);
	return m_Previous[k] == true && m_Current[k] == false;
}
