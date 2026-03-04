#include "CSV_Parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

REC::CSVParser::CSVParser(char separator)
	:m_Separator(separator)
{
}

bool REC::CSVParser::LoadFromFile(const std::string& path)
{
	m_Header.clear();
	m_Rows.clear();
	m_ColumnIndex.clear();

	std::ifstream file(path);
	if (!file.is_open())
		return false;

	std::string line;

	// read Header
	if (!std::getline(file, line))
		return false;

	m_Header = ParseLine(line, m_Separator);

	for (size_t i{}; i < m_Header.size(); ++i)
		m_ColumnIndex[m_Header[i]] = i;

	size_t keyCol = m_ColumnIndex.find("key")->second;
	while (std::getline(file, line))
	{
		auto ParsedLine = ParseLine(line, m_Separator);
		m_Rows[ParsedLine[keyCol]] = ParsedLine;
	}

	return true;
}

std::string REC::CSVParser::GetRaw(const std::string& key, const std::string& data) const
{
	auto col = m_ColumnIndex.find(data);
	if (col == m_ColumnIndex.end())
		throw std::runtime_error("Invalid Column Name");
	return m_Rows.at(key).at(col->second);
}

int REC::CSVParser::GetInt(const std::string& key, const std::string& data) const
{
	return std::stoi(GetRaw(key, data));
}

float REC::CSVParser::GetFloat(const std::string& key, const std::string& data) const
{
	return std::stof(GetRaw(key, data));
}

std::vector<std::string> REC::CSVParser::GetKeys() const
{
	std::vector<std::string> keys{};
	for (const auto& row : m_Rows)
		keys.push_back(row.first);
	
	return keys;
}

std::vector<std::string> REC::CSVParser::ParseLine(const std::string& line, char separator) const
{
	std::vector<std::string> result{};
	std::string cell{};
	std::stringstream ss(line);

	while (std::getline(ss, cell, separator))
		result.push_back(cell);

	return result;
}
