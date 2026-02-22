#pragma once

namespace REC
{
	struct ResourceDesc
	{
		const char* name = "";
		const char* filePath = ""; // relative to the data folder
		virtual ~ResourceDesc() = default;
	protected:
		ResourceDesc() = default;
	};

	struct TextureResourceDesc : public ResourceDesc
	{
		TextureResourceDesc() = default;
		virtual ~TextureResourceDesc() = default;
	};

	struct FontResourceDesc : public ResourceDesc
	{
		FontResourceDesc() = default;
		virtual ~FontResourceDesc() = default;
		uint8_t size = 0;
	};

	struct SoundResourceDesc : public ResourceDesc
	{
		SoundResourceDesc() = default;
		virtual ~SoundResourceDesc() = default;
	};
}