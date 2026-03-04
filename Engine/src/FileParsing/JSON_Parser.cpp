#include "JSON_Parser.h"
#include <nlohmann/json.hpp>

class REC::JSONParser::Impl
{
public:
	

private:
	nlohmann::json m_Json{};
};

REC::JSONParser::JSONParser()
	: m_impl{std::make_unique<Impl>()}
{
}

// destructor must be in cpp file for pimpl to work
REC::JSONParser::~JSONParser() = default;

bool REC::JSONParser::LoadFromFile(const std::string&)
{
	return false;
}

std::string REC::JSONParser::GetRaw(const std::string& , const std::string& ) const
{
	return std::string();
}

int REC::JSONParser::GetInt(const std::string& , const std::string& ) const
{
	return 0;
}

float REC::JSONParser::GetFloat(const std::string& , const std::string& ) const
{
	return 0.0f;
}

std::vector<std::string> REC::JSONParser::GetKeys() const
{
	return std::vector<std::string>();
}
