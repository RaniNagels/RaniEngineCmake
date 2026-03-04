#pragma once
#include <string>
#include <vector>

namespace REC
{
	class IParser
	{
	public:
		explicit IParser() = default;
		~IParser() = default;

		IParser(const IParser&) = delete;
		IParser(IParser&&) = delete;
		IParser& operator= (const IParser&) = delete;
		IParser& operator= (const IParser&&) = delete;

		virtual bool LoadFromFile(const std::string& path) = 0;

		virtual std::string GetRaw(const std::string& key, const std::string& data) const = 0;
		virtual int GetInt(const std::string& key, const std::string& data) const = 0;
		virtual float GetFloat(const std::string& key, const std::string& data) const = 0;

		virtual std::vector<std::string> GetKeys() const = 0;
	};
}