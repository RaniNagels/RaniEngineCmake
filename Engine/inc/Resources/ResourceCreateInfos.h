#pragma once
#include <string>
#include <cstdint>
#include <optional>
#include <DataLoadTypes.h>
#include <RECColor.h>

namespace REC
{
	using SoundID = unsigned int;

	struct ResourceCreateInfo
	{
		std::string name = ""; // the internal name for this resource. how will you refer to this resource in Game
		std::string filePath = ""; // relative to the data folder
		virtual ~ResourceCreateInfo() = default;

		// implement create virtual function to avoid creating everything in the resourceManager
		virtual bool Create() const = 0;

	protected:
		ResourceCreateInfo() = default;
	};

	struct TextureResourceCreateInfo : public ResourceCreateInfo
	{
		TextureResourceCreateInfo() = default;
		virtual ~TextureResourceCreateInfo() = default;
		std::optional<Color> transparentColor;

		virtual bool Create() const override;
	};

	struct FontResourceCreateInfo : public ResourceCreateInfo
	{
		FontResourceCreateInfo() = default;
		virtual ~FontResourceCreateInfo() = default;
		
		virtual bool Create() const override;

		uint8_t size = 0;
	};

	struct SoundResourceCreateInfo : public ResourceCreateInfo
	{
		SoundResourceCreateInfo() = default;
		virtual ~SoundResourceCreateInfo() = default;

		virtual bool Create() const override;

		SoundID id{};
	};

	// name not required
	struct FileResourceCreateInfo : public ResourceCreateInfo
	{
		FileResourceCreateInfo() = default;
		virtual ~FileResourceCreateInfo() = default;

		virtual bool Create() const override;

		// the datatypes in the file that need to be extracted!
		LoadTypes dataTypes{};
	};
}