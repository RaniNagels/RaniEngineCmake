#pragma once
#include <Resources/ResourceCreateInfos.h>
#include <LevelInfo.h>

namespace REC
{
	class IResourceManager 
	{
	public:
		virtual ~IResourceManager() = default;
		virtual bool AddResource(const ResourceCreateInfo& resource) = 0;
		virtual LevelInfo* GetLevelInfo(const std::string& key) const = 0;
	};
}