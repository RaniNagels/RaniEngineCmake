#include <SDL3_ttf/SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"

namespace fs = std::filesystem;

void REC::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_ParserFactory = std::make_unique<ParserFactory>();
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
	else if (typeid(resource) == typeid(SpriteDataResourceCreateInfo))
	{
		const auto tfdesc = static_cast<const SpriteDataResourceCreateInfo&>(resource);
		if (m_SpriteResources.find(tfdesc.name) != m_SpriteResources.end())
		{
			assert(false && "Name already exists in Sprite Resources");
			return false;
		}

		auto parser = m_ParserFactory->RequestParser(tfdesc.type);
		if (tfdesc.type == FileResourceCreateInfo::FileType::CSV)
			m_ParserFactory->SetCSVSeparator(tfdesc.separator);
		if (!parser->LoadFromFile(GetFullPath(tfdesc.filePath)))
			throw std::runtime_error("failed to load file");

		std::unordered_map<std::string, FrameInfo> sprites{};
		for (auto key : parser->GetKeys())
		{
			FrameInfo info{};
			info.key = key;
			if (sprites.find(info.key) != sprites.end())
			{
				assert(false && "key already exists in Sprite Resources");
				return false;
			}
			float x1 = parser->GetFloat(key, "x1");
			float y1 = parser->GetFloat(key, "y1");
			float x2 = parser->GetFloat(key, "x2");
			float y2 = parser->GetFloat(key, "y2");
			info.pixelRegion = TextureRegion(x1, y1, x2, y2);

			sprites.insert({ info.key, info });
		}
		m_SpriteResources.insert({ tfdesc.name, sprites });
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
