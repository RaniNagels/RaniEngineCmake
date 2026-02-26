#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../inc/Singleton.h"

namespace REC
{
	class FileReader final : public Singleton<FileReader>
	{
	public:
		//~FileReader() = default;
		//
		//FileReader(const FileReader& other) = delete;
		//FileReader(FileReader&& other) = delete;
		//FileReader& operator=(const FileReader& other) = delete;
		//FileReader& operator=(FileReader&& other) = delete;

		// needs the full filePath! not a relative one
		bool LoadFromFile(const std::string& filePath, char separator = ',');

		size_t RowCount() const { return m_Rows.size(); }
		const std::string& Get(size_t row, const std::string& columnName) const;

	private:
		std::vector<std::string> m_Header{};
		std::vector<std::vector<std::string>> m_Rows{};
		std::unordered_map<std::string, size_t> m_ColumnIndex{};

		std::vector<std::string> ParseLine(const std::string& line, char separator) const;
	};
}