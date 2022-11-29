#include "JsonParser.hpp"

JsonParser::JsonParser()
{
    

}

JsonParser::~JsonParser()
{

}
void JsonParser::parse(){
std::ifstream input("/home/jonathan/Desktop/project/backEnd/nodeServer/message.json");
if (!input) std::cerr << "Could not open the file!" << std::endl;

}