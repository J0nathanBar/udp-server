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

bool JsonParser::convertJson(bool transmitter, std::string &confpath, int &chunkSize, int &blockSize, std::string &srcPath)
{

    std::ifstream input(confpath);

    if (!input)
    {
        std::cerr << "Could not open the file!" << std::endl;
        return false;
    }
    nlohmann::json data = nlohmann::json::parse(input);
    srcPath = data["srcPath"].get<std::string>();
    if (transmitter)
    {
        chunkSize = data["filePacket"].get<int>();
        blockSize = data["blockSize"].get<int>();
    }

    return true;
}
