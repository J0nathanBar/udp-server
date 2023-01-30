#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#pragma once
#include "ModifiedFile.hpp"
#include "FilePacket.hpp"
#include "DataHeader.hpp"
class FileParser
{
public:
    FileParser();
    ~FileParser();
    void deSerialize(const std::string &, ModifiedFile &);
    std::string serialize(const ModifiedFile &);
    void deSerialize(const std::string &, FilePacket &);
    std::string serialize(const FilePacket &);
    void deSerialize(const std::string &, DataHeader &);
    std::string serialize(const DataHeader &);

private:
};

#endif