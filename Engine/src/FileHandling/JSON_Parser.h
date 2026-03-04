#pragma once
#include <string>
#include "IParser.h"
#include <memory>

namespace REC
{
	class JSONParser final : public IParser
	{
	public:
		explicit JSONParser();
		~JSONParser() = default;

		JSONParser(const JSONParser&) = delete;
		JSONParser(JSONParser&&) = delete;
		JSONParser& operator= (const JSONParser&) = delete;
		JSONParser& operator= (const JSONParser&&) = delete;

		virtual bool LoadFromFile(const std::string& path) override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;
	};
}