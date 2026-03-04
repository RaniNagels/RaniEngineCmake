#pragma once
#include "IParser.h"
#include <memory>

namespace REC
{
	// TODO: complete implementation
	class JSONParser final : public IParser
	{
	public:
		explicit JSONParser();
		~JSONParser();

		JSONParser(const JSONParser&) = delete;
		JSONParser(JSONParser&&) = delete;
		JSONParser& operator= (const JSONParser&) = delete;
		JSONParser& operator= (const JSONParser&&) = delete;

		virtual bool LoadFromFile(const std::string& path) override;

		virtual std::string GetRaw(const std::string& key, const std::string& data) const override;
		virtual int GetInt(const std::string& key, const std::string& data) const override;
		virtual float GetFloat(const std::string& key, const std::string& data) const override;

		virtual std::vector<std::string> GetKeys() const override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;
	};
}