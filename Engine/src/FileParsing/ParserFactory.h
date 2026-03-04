#pragma once
#include "IParser.h"
#include <ResourceCreateInfos.h>
#include <memory>
#include <unordered_map>

namespace REC
{
	class ParserFactory final
	{
	public:
		explicit ParserFactory() = default;
		~ParserFactory() = default;

		ParserFactory(const ParserFactory&) = delete;
		ParserFactory(ParserFactory&&) = delete;
		ParserFactory& operator= (const ParserFactory&) = delete;
		ParserFactory& operator= (const ParserFactory&&) = delete;

		IParser* RequestParser(FileResourceCreateInfo::FileType type);
		void SetCSVSeparator(char separator);

	private:
		std::unordered_map<FileResourceCreateInfo::FileType, std::unique_ptr<IParser>> m_Parsers{};
	};
}