#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

bool REC::FileReader::LoadFromFile(const std::string& filePath, char separator)
{
	m_Header.clear();
	m_Rows.clear();
	m_ColumnIndex.clear();
	
	std::ifstream file(filePath);
	if (!file.is_open())
		return false;

	std::string line;

	// read Header
	if (!std::getline(file, line))
		return false;

	m_Header = ParseLine(line, separator);

	for (size_t i{}; i < m_Header.size(); ++i)
		m_ColumnIndex[m_Header[i]] = i;

	while (std::getline(file, line))
		m_Rows.push_back(ParseLine(line, separator));

	return true;
}

const std::string& REC::FileReader::Get(size_t row, const std::string& columnName) const
{
	auto col = m_ColumnIndex.find(columnName);
	if (col == m_ColumnIndex.end())
		throw std::runtime_error("Invalid Column Name");
	return m_Rows.at(row).at(col->second);
}

std::vector<std::string> REC::FileReader::ParseLine(const std::string& line, char separator) const
{
	std::vector<std::string> result{};
	std::string cell{};
	std::stringstream ss(line);

	while (std::getline(ss, cell, separator))
		result.push_back(cell);

	return result;
}
