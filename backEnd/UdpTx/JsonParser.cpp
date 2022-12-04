#include "JsonParser.hpp"

JsonParser::JsonParser()
{
}

JsonParser::~JsonParser()
{
}
std::string JsonParser::parse(std::string path)
{
    std::ifstream input(path);
    if (!input)
    {
        std::cerr << "Could not open the file!" << std::endl;
        return "";
    }

    nlohmann::json data = nlohmann::json::parse(input);
    std::cout << data << std::endl;
    return data.dump();
}