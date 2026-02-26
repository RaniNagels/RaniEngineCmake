#include <SDL3_ttf/SDL_ttf.h>
#include "../inc/ResourceManager.h"
#include "Renderer.h"
#include "FileReader.h"

namespace fs = std::filesystem;

void REC::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_DataPath = dataPath;

	if (!TTF_Init())
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
}

void REC::ResourceManager::Destroy()
{
	m_FontResources.clear();
	m_TextureResources.clear();
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

		const auto fullPath = (m_DataPath / tdesc.filePath).string();
		m_TextureResources.insert({ tdesc.name, std::make_unique<Texture2D>(fullPath) });
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

		const auto fullPath = (m_DataPath / fdesc.filePath).string();
		m_FontResources.insert({ fdesc.name, std::make_unique<Font>(fullPath, fdesc.size) });
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
		auto& FR = FileReader::GetInstance();
		const auto fullPath = (m_DataPath / tfdesc.filePath).string();
		if (!FR.LoadFromFile(fullPath, tfdesc.separator))
			throw std::runtime_error("failed to load file");

		std::unordered_map<std::string, SpriteInfo> sprites{};
		for (size_t r{}; r < FR.RowCount(); ++r)
		{
			SpriteInfo info{};
			info.key = FR.Get(r, "key");
			if (sprites.find(info.key) != sprites.end())
			{
				assert(false && "key already exists in Sprite Resources");
				return false;
			}
			float x1 = float(stoi(FR.Get(r, "x1")));
			float y1 = float(stoi(FR.Get(r, "y1")));
			float x2 = float(stoi(FR.Get(r, "x2")));
			float y2 = float(stoi(FR.Get(r, "y2")));
			info.pixelRegion = TextureRegion(x1, y1, x2, y2);

			sprites.insert({ info.key, info });
		}
		m_SpriteResources.insert({ tfdesc.name, sprites });
		return true;
	}
	else
		assert("Resource could not be added to ResourceManager: No Type Defined");

	return false;
}
