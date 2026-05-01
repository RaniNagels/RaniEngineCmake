#pragma once
#include <string>
#include <fstream>

#include "FileParsing/JSON_Parser.h"

#include <DataLoadTypes.h>

namespace REC
{
	class DataFile final
	{
	public:
		explicit DataFile(const std::string& filePath);
		~DataFile();

		DataFile(const DataFile&) = delete;
		DataFile(DataFile&&) = delete;
		DataFile& operator= (const DataFile&) = delete;
		DataFile& operator= (const DataFile&&) = delete;

		void Parse(LoadTypes types);

		template <typename T>
		T* GetResource(const std::string& key)
		{
			if constexpr (std::is_same_v<T, FrameInfo>)
			{
				return GetResourceFromMap<T>(m_FrameInfos, key);
			}
			else if constexpr (std::is_same_v<T, AnimationInfo>)
			{
				return GetResourceFromMap<T>(m_AnimationInfos, key);
			}
			else if constexpr (std::is_same_v<T, TextureFontInfo>)
			{
				return GetResourceFromMap<T>(m_TextureFontInfos, key);
			}

			// unreachable code?!
			//assert(false && "Requested resource type is not supported!");
			//return nullptr;
		}

	private:
		template <typename T>
		T* GetResourceFromMap(std::unordered_map<std::string, T>& in, const std::string& key)
		{
			auto it = in.find(key);
			if (it != in.end())
				return &it->second;

			assert(false && "Requested resource type is not supported!");
			return nullptr;
		}

		std::ifstream m_FileStream;
		std::unique_ptr<JSONParser> m_Parser;

		std::unordered_map<std::string, FrameInfo> m_FrameInfos;
		std::unordered_map<std::string, AnimationInfo> m_AnimationInfos;
		std::unordered_map<std::string, TextureFontInfo> m_TextureFontInfos;
	};
}