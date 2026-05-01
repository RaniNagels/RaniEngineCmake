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
#include <Input/InputSystem.h>
#include <SceneManager.h>
#include <Events/EventSystem.h>
#include <Events/EventBroadcaster.h>
#include <IRenderer.h>
#include "Renderer.h"
#include "Resources/ResourceManager.h"
#include "TimeSystem.h"
#include "Window.h"
#include "CollisionSystem.h"
#include "ServiceLocator.h"
#include "Sound/SDL_SoundSystem.h"

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
	: IEngine()
{
	PrintSDLVersion();
	
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) // the same as SDL_Init(/*..*/)
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = std::make_unique<Window>("Programming 4 assignment", 1024, 576);

	Renderer::GetInstance().Init(m_pWindow->GetSDLWindow());
	ResourceManager::GetInstance().Init(dataPath);

	m_pTimeSystem = std::make_unique<TimeSystem>();
	m_pTimeSystem->SetFrameRate(60);
	m_pSceneManager = std::make_unique<SceneManager>();
	m_pInputSystem = std::make_unique<InputSystem>();
	m_pEventSystem = std::make_unique<EventSystem>();
	m_pCollisionSystem = std::make_unique<CollisionSystem>();
	EventBroadcaster::SetEventSystem(m_pEventSystem.get());
	EventBroadcaster::SetCollisionSystem(m_pCollisionSystem.get());

	m_EngineContext.sceneManager = m_pSceneManager.get();
	m_EngineContext.eventSystem = m_pEventSystem.get();
	m_EngineContext.inputSystem = m_pInputSystem.get();
	m_EngineContext.renderer = &Renderer::GetInstance();
	m_EngineContext.resourceManager = &ResourceManager::GetInstance();

	ServiceLocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>());
}

REC::Engine::~Engine()
{
	Renderer::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy(); // must destroy the sound resources before the mixer!!
	ServiceLocator::GetSoundSystem().Destroy();
	m_pWindow->Destroy();
	SDL_Quit();
}

void REC::Engine::Run(void(*load)(IEngine*))
{
	if (load) load(this);
	m_pWindow->DisplayWindow();

#ifndef __EMSCRIPTEN__
	while (!m_pInputSystem->ShouldQuit())
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

	m_pInputSystem->ProcessInput(m_pTimeSystem->GetDeltaTime());
	m_pSceneManager->Update(m_pTimeSystem->GetDeltaTime());
	m_pCollisionSystem->CheckCollisions(m_pSceneManager->GetActiveScene());
	m_pEventSystem->ProcessEvents();
	m_pSceneManager->Render();

	std::this_thread::sleep_for(m_pTimeSystem->GetSleepTime());
}

void REC::Engine::SetEngineSettings(const EngineSettings& data)
{
	m_pTimeSystem->SetFrameRate(data.frameRate);
	m_pWindow->SetSize(data.windowWidth, data.windowHeight);
	m_pWindow->SetTitle(data.windowTitle);
}


// Engine Creation functions (global)
extern "C"
{
	REC::IEngine* REC::CreateEngine(const std::filesystem::path& dataPath)
	{
		return new REC::Engine(dataPath);
	}

	void REC::DestroyEngine(REC::IEngine* engine)
	{
		delete engine;
		engine = nullptr;
	}
}
