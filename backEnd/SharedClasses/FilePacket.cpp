#include "FilePacket.hpp"

FilePacket::FilePacket()
{
}
FilePacket::FilePacket(std::string id, std::string data, unsigned long index, unsigned long last) : _fileId(id), _data(data), _index(index), _lastPacket(last)
{
}

FilePacket::~FilePacket()
{
}
std::string FilePacket::getData()
{
    return _data;
}
std::string FilePacket::getId()
{
    return _fileId;
}
unsigned long FilePacket::getIndex()
{
    return _index;
}
unsigned long FilePacket::getLastPacket()
{
    return _lastPacket;
}