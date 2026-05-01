#include "ResourceManager.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <Renderer.h>

namespace fs = std::filesystem;

void REC::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_DataPath = dataPath;

	if (!TTF_Init())
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
}

void REC::ResourceManager::Destroy()
{
	m_FontResources.clear(); // must be cleared before TTF_Quit();
	m_SoundResources.clear(); // must be cleared before mixer is destroyed in SDL_SoundSystem
	TTF_Quit();
}

bool REC::ResourceManager::AddResource(const ResourceCreateInfo& resource)
{
	return resource.Create();
}

std::string REC::ResourceManager::GetFullPath(const std::string& relativePath)
{
	return (m_DataPath / relativePath).string();
}
