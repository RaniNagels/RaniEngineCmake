#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include "Singleton.h"
#include <ResourceCreateInfos.h>

#include <unordered_map>
#include <stdexcept>
#include <assert.h>

#include "Resources/Font.h"	
#include "Resources/Texture2D.h"
#include "Resources/DataFile.h"
#include "FrameInfo.h"
#include "AnimationInfo.h"
#include "TextureFontInfo.h"

#include <FileParsing/JSON_Parser.h>

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
		T* GetResource(const std::string& key)
		{
			if constexpr (std::is_same_v<T, Texture2D>)
			{
				return GetResourceFromMap<T>(m_TextureResources, key);
			}
			else if constexpr (std::is_same_v<T, Font>)
			{
				return GetResourceFromMap<T>(m_FontResources, key);
			}

			// unreachable code?!
			//assert(false && "Requested resource type is not supported!");
			//return nullptr;
		}

		template <typename T>
		T* GetResourceFromDataFile(const std::string& fileKey, const std::string& key)
		{
			auto it = m_DataFileResources.find(fileKey);
			if (it != m_DataFileResources.end())
				return it->second->GetResource<T>(key);

			assert(false && "Requested resource type is not supported!");
			return nullptr;
		}

	private:
		template <typename T>
		T* GetResourceFromMap(std::unordered_map<std::string, std::unique_ptr<T>>& in, const std::string& key)
		{
			auto it = in.find(key);
			if (it != in.end())
				return it->second.get();

			assert(false && "Requested resource type is not supported!");
			return nullptr;
		}

		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::filesystem::path m_DataPath = "";

		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_TextureResources{};
		std::unordered_map<std::string, std::unique_ptr<Font>> m_FontResources{};
		std::unordered_map<std::string, std::unique_ptr<DataFile>> m_DataFileResources{};

		std::unique_ptr<JSONParser> m_Parser{};

		std::string GetFullPath(const std::string& relativePath);
	};
}
