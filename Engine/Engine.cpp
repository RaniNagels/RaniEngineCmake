#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Engine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "src/TimeSystem.h"
#include "src/Window.h"

#include <thread>
#include <chrono>

void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#include <thread>

void LoopCallback(void* arg)
{
	static_cast<REC::Engine*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
	// version = IMG_Version();
	// LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

REC::Engine::Engine(const std::filesystem::path& dataPath)
{
	PrintSDLVersion();
	
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = std::make_unique<Window>("Programming 4 assignment", 1024, 576);

	Renderer::GetInstance().Init(m_pWindow->GetSDLWindow());
	ResourceManager::GetInstance().Init(dataPath);

	m_pTimeSystem = std::make_unique<TimeSystem>();
}

REC::Engine::~Engine()
{
	Renderer::GetInstance().Destroy();
	SDL_Quit();
}

void REC::Engine::Run(const std::function<void()>& load)
{
	load();
	m_pWindow->DisplayWindow();

#ifndef __EMSCRIPTEN__
	while (!m_quit)
	{
		RunOneFrame();
	}
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void REC::Engine::RunOneFrame()
{
	m_pTimeSystem->Update();

	m_quit = !InputManager::GetInstance().ProcessInput();
	SceneManager::GetInstance().Update(m_pTimeSystem->GetDeltaTime());
	Renderer::GetInstance().Render();

	std::this_thread::sleep_for(m_pTimeSystem->GetSleepTime());
}
