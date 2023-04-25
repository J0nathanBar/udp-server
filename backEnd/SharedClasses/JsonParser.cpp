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
    // std::cout << data << std::endl;

    return data.dump();
}

bool JsonParser::convertJson(std::string &srcPath)
{
    std::string input;
    std::getline(std::cin, input);
    nlohmann::json data = nlohmann::json::parse(input);
    srcPath = data["srcPath"].get<std::string>();
    return true;
}

bool JsonParser::convertJson(int &chunkSize, int &blockSize, std::string &srcPath)
{
    std::string input;
    std::getline(std::cin, input);
    nlohmann::json data = nlohmann::json::parse(input);
    srcPath = data["srcPath"].get<std::string>();
    chunkSize = data["filePacket"].get<int>();
    blockSize = data["blockSize"].get<int>();
    return true;
}
