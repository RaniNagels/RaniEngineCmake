#pragma once
#include <memory>
#include <unordered_map>
#include <string>

#include "../FrameInfo.h"
#include "../AnimationInfo.h"

namespace REC
{
	class JSONParser final
	{
	public:
		explicit JSONParser();
		~JSONParser();

		JSONParser(const JSONParser&) = delete;
		JSONParser(JSONParser&&) = delete;
		JSONParser& operator= (const JSONParser&) = delete;
		JSONParser& operator= (const JSONParser&&) = delete;

		bool LoadFromFile(const std::string& path);

		bool GetFrames(std::unordered_map<std::string, FrameInfo>& out);
		bool GetAnimations(std::unordered_map<std::string, AnimationInfo>& out);

	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;
	};
}