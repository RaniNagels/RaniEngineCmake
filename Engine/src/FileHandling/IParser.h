#pragma once

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
	};
}