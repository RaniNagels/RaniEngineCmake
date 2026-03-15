#pragma once
#include <string>
#include <cstdint>
#include <DataLoadTypes.h>

namespace REC
{
	class Texture2D;
	class Font;
	class DataFile;

	struct ResourceCreateInfo
	{
		std::string name = ""; // the internal name for this resource. how will you refer to this resource in Game
		std::string filePath = ""; // relative to the data folder
		virtual ~ResourceCreateInfo() = default;

		// TODO: implement create virtual function to avoid creating everything in the resourceManager
		//virtual void Create() = 0;
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

	//struct SoundResourceCreateInfo : public ResourceCreateInfo
	//{
	//	SoundResourceCreateInfo() = default;
	//	virtual ~SoundResourceCreateInfo() = default;
	//};

	// name not required
	struct FileResourceCreateInfo : public ResourceCreateInfo
	{
		FileResourceCreateInfo() = default;
		virtual ~FileResourceCreateInfo() = default;

		// the datatypes in the file that need to be extracted!
		LoadTypes dataTypes{};
	};
}