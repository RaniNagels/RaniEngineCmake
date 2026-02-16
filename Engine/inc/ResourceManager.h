#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"
#include "ResourceDescriptors.h"
#include <unordered_map>
#include <stdexcept>
#include <assert.h>
#include "../src/Font.h"
#include "../src/Texture2D.h"

namespace REC
{
	class Texture2D;
	class Font;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::filesystem::path& data);

		bool AddResource(const ResourceDesc& resource);

		template <typename T>
		T* GetResource(const std::string& name) const
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
			assert("Requested resource type is not supported!" && false);
			return nullptr;
		}

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::filesystem::path m_DataPath;

		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_TextureResources{};
		std::unordered_map<std::string, std::unique_ptr<Font>> m_FontResources{};
	};
}
