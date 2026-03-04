#pragma once
#include "IParser.h"
#include <vector>
#include <unordered_map>

namespace REC
{
	class CSVParser final : public IParser
	{
	public:
		explicit CSVParser(char separator = ',');
		~CSVParser() = default;

		CSVParser(const CSVParser&) = delete;
		CSVParser(CSVParser&&) = delete;
		CSVParser& operator= (const CSVParser&) = delete;
		CSVParser& operator= (const CSVParser&&) = delete;

		virtual bool LoadFromFile(const std::string& path) override;

		virtual std::string GetRaw(const std::string& key, const std::string& data) const override;
		virtual int GetInt(const std::string& key, const std::string& data) const override;
		virtual float GetFloat(const std::string& key, const std::string& data) const override;

		virtual std::vector<std::string> GetKeys() const override;

		void SetSeparator(char separator = ',') { m_Separator = separator; }

	private:
		std::vector<std::string> m_Header{};
		std::unordered_map<std::string, std::vector<std::string>> m_Rows{};
		std::unordered_map<std::string, size_t> m_ColumnIndex{};

		char m_Separator{ ',' };

		std::vector<std::string> ParseLine(const std::string& line, char separator) const;
	};
}