#pragma once
#include <string>
#include <cstdint>

namespace REC
{
	struct ResourceCreateInfo
	{
		std::string name = ""; // the internal name for this resource. how will you refer to this resource in Game
		std::string filePath = ""; // relative to the data folder
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

	// name not required
	struct FileResourceCreateInfo : public ResourceCreateInfo
	{
		FileResourceCreateInfo() = default;
		virtual ~FileResourceCreateInfo() = default;

		enum class LoadTypes : uint8_t
		{
			Frames = 0x01,
			Animations = 0x02,
			TextureFont = 0x04,
		};

		// the datatypes in the file that need to be extracted!
		LoadTypes dataTypes{};
	};

	inline FileResourceCreateInfo::LoadTypes operator|(FileResourceCreateInfo::LoadTypes a, FileResourceCreateInfo::LoadTypes b)
	{
		return static_cast<FileResourceCreateInfo::LoadTypes>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}
	
	inline FileResourceCreateInfo::LoadTypes operator&(FileResourceCreateInfo::LoadTypes a, FileResourceCreateInfo::LoadTypes b)
	{
		return static_cast<FileResourceCreateInfo::LoadTypes>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}
}