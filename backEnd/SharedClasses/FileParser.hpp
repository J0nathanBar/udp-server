#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#pragma once
#include "ModifiedFile.hpp"
#include "FilePacket.hpp"
class FileParser
{
public:
    FileParser();
    ~FileParser();
    void deSerialize(std::string, ModifiedFile &);
    std::string serialize(ModifiedFile &);
    void deSerialize(std::string, FilePacket &);
    std::string serialize(FilePacket &);

private:
};

#endif