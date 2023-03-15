#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#pragma onc
#include "FilePacket.hpp"
#include "DataHeader.hpp"
class FileParser
{
public:
    FileParser();
    ~FileParser();
    bool deSerialize(const std::string &, FilePacket &);
    std::string serialize(const FilePacket &);
    bool deSerialize(const std::string &, DataHeader &);
    std::string serialize(const DataHeader &);

private:
};

#endif