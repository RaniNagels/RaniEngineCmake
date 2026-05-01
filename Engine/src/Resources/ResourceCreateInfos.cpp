#include <Resources/ResourceCreateInfos.h>

#include "ResourceManager.h"
#include "Resources/ResourceTypes/Texture2D.h"
#include "Resources/ResourceTypes/Font.h"
#include "Resources/ResourceTypes/DataFile.h"
#include "Sound/SDL_Sound.h"
#include "Sound/SDL_SoundSystem.h"

#include <ServiceLocator.h>

bool REC::TextureResourceCreateInfo::Create() const
{
	// code to create happens here
	// pass the object and key to the resource manager
	ResourceManager& RM = ResourceManager::GetInstance();
	return RM.AddResource(name, std::make_unique<Texture2D>(RM.GetFullPath(filePath), transparentColor));
}

bool REC::FontResourceCreateInfo::Create() const
{
	ResourceManager& RM = ResourceManager::GetInstance();
	return RM.AddResource(name, std::make_unique<Font>(RM.GetFullPath(filePath), size));
}

bool REC::SoundResourceCreateInfo::Create() const
{
	ResourceManager& RM = ResourceManager::GetInstance();
	// the type of sounds get decided here based on the SoundSystem registered to the service locator.
	if (dynamic_cast<SDL_SoundSystem*>(&ServiceLocator::GetSoundSystem()) != nullptr)
		return RM.AddResource(name, std::make_unique<SDL_Sound>(RM.GetFullPath(filePath)));
	return false; // sound system not supported
}

bool REC::FileResourceCreateInfo::Create() const
{
	ResourceManager& RM = ResourceManager::GetInstance();

	auto dataFile = std::make_unique<DataFile>(RM.GetFullPath(filePath));
	dataFile->Parse(dataTypes);

	return RM.AddResource(name, std::move(dataFile));
}
