#pragma once
#include <vector>
#include <typeindex>

namespace REC
{
	struct ResourceCreateInfo
	{
		const char* name = ""; // the internal name for this resource. how will you refer to this resource in Game
		const char* filePath = ""; // relative to the data folder
		virtual ~ResourceCreateInfo() = default;
	protected:
		ResourceCreateInfo() = default;
	};

	struct TextureResourceCreateInfo : public ResourceCreateInfo
	{
		TextureResourceCreateInfo() = default;
		virtual ~TextureResourceCreateInfo() = default;
	};

	struct FontResourceCreateInfo : public ResourceCreateInfo
	{
		FontResourceCreateInfo() = default;
		virtual ~FontResourceCreateInfo() = default;
		uint8_t size = 0;
	};

	struct SoundResourceCreateInfo : public ResourceCreateInfo
	{
		SoundResourceCreateInfo() = default;
		virtual ~SoundResourceCreateInfo() = default;
	};

	struct FileResourceCreateInfo : public ResourceCreateInfo
	{
		FileResourceCreateInfo() = default;
		virtual ~FileResourceCreateInfo() = default;

		char separator = ',';
	};

	struct SpriteDataResourceCreateInfo : public FileResourceCreateInfo
	{
		SpriteDataResourceCreateInfo() = default;
		virtual ~SpriteDataResourceCreateInfo() = default;
	};
}