#include <SDL3_ttf/SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"

namespace fs = std::filesystem;

void REC::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_Parser = std::make_unique<JSONParser>();
	m_DataPath = dataPath;

	if (!TTF_Init())
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
}

void REC::ResourceManager::Destroy()
{
	m_FontResources.clear(); // must be cleared before TTF_Quit();
	TTF_Quit();
}

bool REC::ResourceManager::AddResource(const ResourceCreateInfo& resource)
{
	if (typeid(resource) == typeid(TextureResourceCreateInfo))
	{
		const auto& tdesc = static_cast<const TextureResourceCreateInfo&>(resource);
		if (m_TextureResources.find(tdesc.name) != m_TextureResources.end())
		{
			assert(false && "Name already exists in Texture Resources");
			return false;
		}

		m_TextureResources.insert({ tdesc.name, std::make_unique<Texture2D>(GetFullPath(tdesc.filePath)) });
		return true;
	}
	else if (typeid(resource) == typeid(FontResourceCreateInfo))
	{
		const auto& fdesc = static_cast<const FontResourceCreateInfo&>(resource);
		if (m_FontResources.find(fdesc.name) != m_FontResources.end())
		{
			assert(false && "Name already exists in Font Resources");
			return false;
		}

		m_FontResources.insert({ fdesc.name, std::make_unique<Font>(GetFullPath(fdesc.filePath), fdesc.size) });
		return true;
	}
	else if (typeid(resource) == typeid(FileResourceCreateInfo))
	{
		const auto fdesc = static_cast<const FileResourceCreateInfo&>(resource);
		if (m_SpriteResources.find(fdesc.name) != m_SpriteResources.end())
		{
			assert(false && "Name already exists in Sprite Resources");
			return false;
		}

		if (!m_Parser->LoadFromFile(GetFullPath(fdesc.filePath)))
			throw std::runtime_error("failed to load file");

		std::unordered_map<std::string, FrameInfo> sprites{};
		if (m_Parser->GetFrames(sprites))
			m_SpriteResources.insert({ fdesc.name, sprites });

		std::unordered_map<std::string, AnimationInfo> animationData{};
		if (m_Parser->GetAnimations(animationData))
			m_AnimationResources.insert({ fdesc.name, animationData });

		return true;
	}
	else
		assert("Resource could not be added to ResourceManager: No Valid Type");

	return false;
}

std::string REC::ResourceManager::GetFullPath(const std::string& relativePath)
{
	return (m_DataPath / relativePath).string();
}
