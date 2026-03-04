#include "ParserFactory.h"
#include "CSV_Parser.h"
#include "JSON_Parser.h"
#include <stdexcept>

REC::ParserFactory::~ParserFactory() = default;

REC::IParser* REC::ParserFactory::RequestParser(FileResourceCreateInfo::FileType type)
{
	// check if parsers already exist in map
	auto parser = m_Parsers.find(type);
	if (parser == m_Parsers.end())
	{
		// create a new parser
		switch (type)
		{
		case FileResourceCreateInfo::FileType::CSV:
			m_Parsers[type] = std::make_unique<CSVParser>();
			break;
		case FileResourceCreateInfo::FileType::JSON:
			m_Parsers[type] = std::make_unique<JSONParser>();
			break;
		case FileResourceCreateInfo::FileType::NONE:
			throw std::runtime_error("Invalid File Type!");
			break;
		}
	}

	return m_Parsers.at(type).get();
}

void REC::ParserFactory::SetCSVSeparator(char separator)
{
	auto CSV_Parser = static_cast<CSVParser*>(m_Parsers.at(FileResourceCreateInfo::FileType::CSV).get());
	CSV_Parser->SetSeparator(separator);
}
