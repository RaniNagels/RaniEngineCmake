#include <SDL3_ttf/SDL_ttf.h>
#include "../inc/ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

namespace fs = std::filesystem;

void REC::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_DataPath = dataPath;

	if (!TTF_Init())
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

bool REC::ResourceManager::AddResource(const ResourceDesc& resource)
{
	switch (resource.GetType())
	{
	case ResourceType::Texture:
	{
		const auto& tdesc = static_cast<const TextureResourceDesc&>(resource);

		if (m_TextureResources.find(tdesc.name) != m_TextureResources.end())
		{
			assert(false && "Name already exists in Texture Resources");
			return false;
		}

		const auto fullPath = (m_DataPath / tdesc.filePath).string();
		m_TextureResources.insert({ tdesc.name, std::make_unique<Texture2D>(fullPath) });
		return true;
	}

	case ResourceType::Font:
	{
		const auto& fdesc = static_cast<const FontResourceDesc&>(resource);

		if (m_FontResources.find(fdesc.name) != m_FontResources.end())
		{
			assert(false && "Name already exists in Font Resources");
			return false;
		}

		const auto fullPath = (m_DataPath / fdesc.filePath).string();
		m_FontResources.insert({ fdesc.name, std::make_unique<Font>(fullPath, fdesc.size) });
		return true;
	}

	case ResourceType::Unknown:
		assert("Resource could not be added to ResourceManager: No Type Defined");
		break;
	}
	return false;
}
