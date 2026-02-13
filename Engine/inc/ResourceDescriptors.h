#pragma once

namespace REC
{
	enum class ResourceType
	{
		Unknown,
		Texture,
		Font,
	};

	struct ResourceDesc
	{
		const char* name = "";
		const char* filePath = ""; // relative to the data folder
		ResourceType GetType() const { return type; }

	protected:
		ResourceDesc() = default;
		ResourceType type = ResourceType::Unknown;
	};

	struct TextureResourceDesc : public ResourceDesc
	{
		TextureResourceDesc() { type = ResourceType::Texture; }
	};

	struct FontResourceDesc : public ResourceDesc
	{
		FontResourceDesc() { type = ResourceType::Font; }
		uint8_t size = 0;
	};
}