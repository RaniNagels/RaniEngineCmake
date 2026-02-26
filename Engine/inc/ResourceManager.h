#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"
#include "ResourceCreateInfos.h"
#include <unordered_map>
#include <stdexcept>
#include <assert.h>
#include "../src/Font.h"		  // TODO: should not be here
#include "../src/Texture2D.h"	  // TODO: should not be here
#include "../src/SpriteInfo.h"	  // TODO: should not be here

namespace REC
{
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::filesystem::path& data);
		void Destroy();

		bool AddResource(const ResourceCreateInfo& resource);

		// TODO: re add const to the GetResource methods
		template <typename T>
		T* GetResource(const std::string& name)
		{
			if constexpr (std::is_same_v<T, Texture2D>)
			{
				auto it = m_TextureResources.find(name);
				if (it != m_TextureResources.end())
					return it->second.get();
			}
			else if constexpr (std::is_same_v<T, Font>)
			{
				auto it = m_FontResources.find(name);
				if (it != m_FontResources.end())
					return it->second.get();
			}
			assert(false && "Requested resource type is not supported!");
			return nullptr;
		}

		template <typename T>
		T* GetResource(const std::string& filename, const std::string& key)
		{
			if constexpr (std::is_same_v<T, SpriteInfo>)
			{
				auto fileIt = m_SpriteResources.find(filename);
				if (fileIt != m_SpriteResources.end())
				{
					auto spriteIt = fileIt->second.find(key);
					if (spriteIt != fileIt->second.end())
						return &spriteIt->second;
				}
			}
			assert(false && "Requested resource type is not supported!");
			return nullptr;
		}

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::filesystem::path m_DataPath = "";

		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_TextureResources{};
		std::unordered_map<std::string, std::unique_ptr<Font>> m_FontResources{};

		std::unordered_map<
			std::string, // filekey
			std::unordered_map<
				std::string, // spritekey
				SpriteInfo>
		> m_SpriteResources{};
	};
}
