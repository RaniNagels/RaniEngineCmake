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

bool REC::JSONParser::LoadFromFile(const std::string&)
{
	return false;
}
