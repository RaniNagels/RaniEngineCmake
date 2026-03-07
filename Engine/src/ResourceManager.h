#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"
#include <ResourceCreateInfos.h>

#include <unordered_map>
#include <stdexcept>
#include <assert.h>
#include "Font.h"	
#include "Texture2D.h"
#include "FrameInfo.h"
#include "AnimationInfo.h"

#include <FileParsing/JSON_Parser.h>
#include "TextureFontInfo.h"

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
				auto it = m_TextureResources.find(key);
				if (it != m_TextureResources.end())
					return it->second.get();
				else
					assert(false && "Requested resource type is not supported!");
			}
			else if constexpr (std::is_same_v<T, Font>)
			{
				auto it = m_FontResources.find(key);
				if (it != m_FontResources.end())
					return it->second.get();
				else 
					assert(false && "Requested resource type is not supported!");
			}
			else if constexpr (std::is_same_v<T, FrameInfo>)
			{
				auto it = m_FrameResources.find(key);
				if (it != m_FrameResources.end())
					return &it->second;
				else
					assert(false && "Requested resource type is not supported!");
			}
			else if constexpr (std::is_same_v<T, AnimationInfo>)
			{
				auto it = m_AnimationResources.find(key);
				if (it != m_AnimationResources.end())
					return &it->second;
				else
					assert(false && "Requested resource type is not supported!");
			}
			else if constexpr (std::is_same_v<T, TextureFontInfo>)
			{
				auto it = m_AnimationResources.find(key);
				if (it != m_AnimationResources.end())
					return &it->second;
				else
					assert(false && "Requested resource type is not supported!");
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

		std::unordered_map<std::string, FrameInfo> m_FrameResources{};
		std::unordered_map<std::string, AnimationInfo> m_AnimationResources{};
		std::unordered_map<std::string, TextureFontInfo> m_TextureFontResources{}; 

		std::unique_ptr<JSONParser> m_Parser{};

		std::string GetFullPath(const std::string& relativePath);
	};
}
