#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include "Singleton.h"
#include <Resources/IResourceManager.h>

#include <unordered_map>
#include <stdexcept>
#include <assert.h>

#include "Resources/ResourceTypes/Font.h"	
#include "Resources/ResourceTypes/Texture2D.h"
#include "Resources/ResourceTypes/DataFile.h"
#include "Resources/ResourceTypes/ISound.h"
#include "FrameInfo.h"
#include "AnimationInfo.h"
#include "TextureFontInfo.h"

namespace REC
{
	class ResourceManager final : public Singleton<ResourceManager>, public IResourceManager
	{
	public:
		void Init(const std::filesystem::path& data);
		void Destroy();

		virtual bool AddResource(const ResourceCreateInfo& resource) override;

		template <typename T>
		bool AddResource(const std::string& key, std::unique_ptr<T> resource)
		{
			if constexpr (std::is_same_v<T, Texture2D>)
			{
				if (m_TextureResources.find(key) != m_TextureResources.end())
				{
					assert(false && "Name already exists in Texture Resources");
					return false;
				}
				m_TextureResources.insert({ key, std::move(resource) });
				return true;
			}
			else if constexpr (std::is_same_v<T, Font>)
			{
				if (m_FontResources.find(key) != m_FontResources.end())
				{
					assert(false && "Name already exists in Font Resources");
					return false;
				}
				m_FontResources.insert({ key, std::move(resource) });
				return true;
			}
			else if constexpr (std::is_same_v<T, DataFile>)
			{
				if (m_DataFileResources.find(key) != m_DataFileResources.end())
				{
					assert(false && "Name already exists in DataFile Resources");
					return false;
				}
				m_DataFileResources.insert({ key, std::move(resource) });
				return true;
			}
			else if constexpr (std::derived_from<T, ISound>)
			{
				if (m_SoundResources.find(key) != m_SoundResources.end())
				{
					assert(false && "Name already exists in Sound Resources");
					return false;
				}
				m_SoundResources.insert({ key, std::move(resource) });
				return true;
			}

			// unreachable code?!
			//assert(false && "Requested resource type is not supported!");
			//return false;
		}

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
			else if constexpr (std::is_same_v<T, ISound>)
			{
				return GetResourceFromMap<T>(m_SoundResources, key);
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

		std::string GetFullPath(const std::string& relativePath);

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
		std::unordered_map<std::string, std::unique_ptr<ISound>> m_SoundResources{};
	};
}
