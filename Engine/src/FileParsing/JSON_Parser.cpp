#include "JSON_Parser.h"

#include <nlohmann/json.hpp>
#include <fstream>

class REC::JSONParser::Impl
{
public:
	bool LoadFromFile(const std::string& filePath)
	{
		std::ifstream f(filePath);
	
		return LoadFromFile(f);
	}

	bool LoadFromFile(std::ifstream& file)
	{
		if (!file.is_open()) return false;

		file >> m_Json;
		if (m_Json.empty()) return false;

		return true;
	}

	bool ParseFrames(std::unordered_map<std::string, FrameInfo>& out)
	{
		auto frames = m_Json["frames"];
		if (frames.empty()) return false;
		for (auto& [key, value] : frames.items())
		{
			if (out.find(key) != out.end())
			{
				assert(false && "Name already exists in frame resources");
				continue;
			}
			FrameInfo info{};
			info.textureFileKey = value["sheet"];
			info.pixelRegion.x = value["x"];
			info.pixelRegion.y = value["y"];
			info.pixelRegion.width = value["w"];
			info.pixelRegion.height = value["h"];
			
			out.insert({ key, info });
		}
		return true;
	}

	bool ParseAnimations(std::unordered_map<std::string, AnimationInfo>& out)
	{
		auto animationData = m_Json["animations"];
		if (animationData.empty()) return false;
		for (auto& [key, value] : animationData.items())
		{
			if (out.find(key) != out.end())
			{
				assert(false && "Name already exists in animation resources");
				continue;
			}
			AnimationInfo info{};
			info.animationKey = key;
			info.duration = value["duration"];
			info.frameKeys = value["frames"];
			info.loop = value["loop"];

			out.insert({ key, info });
		}
		return true;
	}

	bool ParseTextureFonts(std::unordered_map<std::string, TextureFontInfo>& out)
	{
		auto textureFontData = m_Json["texture_font"];
		if (textureFontData.empty()) return false;
		for (auto& [key, value] : textureFontData.items())
		{
			if (out.find(key) != out.end())
			{
				assert(false && "Name already exists in textureFont resources");
				continue;
			}
			TextureFontInfo info{};
			
			for (auto& [character, frameKey] : value.items())
			{
				info.glyphs[character[0]] = frameKey;
			}

			out.insert({ key, info });
		}
		return true;
	}

	bool ParseLevels(std::unordered_map<std::string, LevelInfo>& out)
	{
		auto levelData = m_Json["levels"];
		if (levelData.empty()) return false;

		for (const auto& value : levelData)
		{
			LevelInfo info{};

			info.id = value["id"].get<uint8_t>();

			info.player1StartCell = {
				value["player1Start"][0].get<uint8_t>(),
				value["player1Start"][1].get<uint8_t>()
			};

			info.player2StartCell = {
				value["player2Start"][0].get<uint8_t>(),
				value["player2Start"][1].get<uint8_t>()
			};

			for (const auto& b : value["bricks"])
				info.bricks.emplace_back(b[0], b[1]);

			for (const auto& e : value["enemies"])
				info.enemies.emplace_back(
					CellCo{
						e["x"].get<uint8_t>(),
						e["y"].get<uint8_t>()
					},
					e["type"].get<std::string>()
				);

			info.exitBrickIndex = value["exitBrick"].get<uint16_t>();

			info.powerUps = {
				value["powerUp"]["brick"].get<uint8_t>(),
				value["powerUp"]["type"]
			};

			out.emplace(std::to_string(info.id), std::move(info));
		}
		return true;
	}

private:
	nlohmann::json m_Json{};
};

REC::JSONParser::JSONParser()
	: m_impl{std::make_unique<Impl>()}
{ }

// destructor must be in cpp file for pimpl to work
REC::JSONParser::~JSONParser() = default;

bool REC::JSONParser::LoadFromFile(const std::string& path)
{
	return m_impl->LoadFromFile(path);
}

bool REC::JSONParser::LoadFromFile(std::ifstream& file)
{
	return m_impl->LoadFromFile(file);
}

bool REC::JSONParser::GetFrames(std::unordered_map<std::string, FrameInfo>& out)
{
	return m_impl->ParseFrames(out);
}

bool REC::JSONParser::GetAnimations(std::unordered_map<std::string, AnimationInfo>& out)
{
	return m_impl->ParseAnimations(out);
}

bool REC::JSONParser::GetTextureFonts(std::unordered_map<std::string, TextureFontInfo>& out)
{
	return m_impl->ParseTextureFonts(out);
}

bool REC::JSONParser::GetLevels(std::unordered_map<std::string, LevelInfo>& out)
{
	return m_impl->ParseLevels(out);
}
