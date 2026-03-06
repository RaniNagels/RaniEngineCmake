#include "JSON_Parser.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "FrameInfo.h"
#include "AnimationInfo.h"

class REC::JSONParser::Impl
{
public:
	bool LoadFromFile(const std::string& filePath)
	{
		std::ifstream f(filePath);

		if (!f.is_open())
			return false;

		f >> m_Json;
		if (m_Json.empty())
			return false;

		return true;
	}

	bool ParseFrames(std::unordered_map<std::string, FrameInfo>& out)
	{
		auto frames = m_Json["frames"];
		if (frames.empty()) return false;
		for (auto& [key, value] : frames.items())
		{
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

	bool ParseAnimation(std::unordered_map<std::string, AnimationInfo>& out)
	{
		auto animationData = m_Json["animations"];
		if (animationData.empty()) return false;
		for (auto& [key, value] : animationData.items())
		{
			AnimationInfo info{};
			info.animationKey = key;
			info.duration = value["duration"];
			info.frameKeys = value["frames"];
			info.loop = value["loop"];

			out.insert({ key, info });
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

bool REC::JSONParser::GetFrames(std::unordered_map<std::string, FrameInfo>& out)
{
	return m_impl->ParseFrames(out);
}

bool REC::JSONParser::GetAnimations(std::unordered_map<std::string, AnimationInfo>& out)
{
	return m_impl->ParseAnimation(out);
}
