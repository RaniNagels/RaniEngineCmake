#include "Window.h"
#include <SDL3/SDL.h>
#include <stdexcept>

REC::Window::Window(const std::string& title, int width, int height)
{
	m_Window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	if (m_Window == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
}

REC::Window::~Window()
{
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
}

void REC::Window::DisplayWindow() const
{
	SDL_ShowWindow(m_Window);
}
