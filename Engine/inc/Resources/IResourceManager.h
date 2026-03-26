#pragma once
#include <Resources/ResourceCreateInfos.h>

namespace REC
{
	class IResourceManager 
	{
	public:
		virtual ~IResourceManager() = default;
		virtual bool AddResource(const ResourceCreateInfo& resource) = 0;
	};
}