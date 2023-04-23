#ifndef JSONPARSER_HPP
#define JSONPARSER_HPP

#pragma once
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include<iostream>
#include <string.h>
class JsonParser
{
public:
    JsonParser();
    ~JsonParser();
    std::string parse(std::string path);
    bool convertJson(bool transmitter, int &chunkSize, int &blockSize, std::string &srcPath);

private:
};

#endif