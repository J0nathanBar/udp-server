#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#pragma once
#include "ModifiedFile.hpp"

class FileParser
{
public:
    FileParser();
    ~FileParser();
    void deSerialize(std::string,ModifiedFile * );
    std::string serialize(ModifiedFile * data);

private:


};

#endif