#pragma once
#include <string>

struct SDL_Window;

namespace REC
{
	// wrapper around SDL_Window
	class Window final
	{
	public:
		Window(const std::string& title, int width, int height);
		~Window();

		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;

		SDL_Window* GetSDLWindow() const { return m_Window; }

		// prevent a black screen as the resources are loading
		void DisplayWindow() const;
		void SetSize(uint16_t width, uint16_t height);
		void SetTitle(const std::string& title);

	private:
		SDL_Window* m_Window; 
		// TODO? unique_ptr? with custom deleter? maybe?
	};
}