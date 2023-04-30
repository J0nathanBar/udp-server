#include "FilePacket.hpp"

FilePacket::FilePacket(unsigned int packetsDecoded, unsigned long headerCreated, unsigned long lastDetected, unsigned long startDecode) : _recovered(std::chrono::system_clock::now()), _packetsDecoded(packetsDecoded), _headerCreated(headerCreated), _lastDetected(lastDetected), _startDecode(startDecode)
{
}
FilePacket::FilePacket(std::string id, std::string data, unsigned long index, unsigned long size, std::string fileName, std::string rootFolder) : _fileId(std::move(id)),
                                                                                                                                                  _data(std::move(data)), _index(index), _size(size), _fileName(std::move(fileName)), _rootFolder(std::move(rootFolder))
{
}

FilePacket::~FilePacket()
{
}
std::string FilePacket::getData()
{
    return _data;
}
std::string FilePacket::getFileData()
{
    return _fileData;
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

void FilePacket::printInfo()
{
    //  std::cout << "id: "<<_fileId << " index: " << _index << " last packet: " << _lastPacket << std::endl;
}
void FilePacket::setLastPacket(unsigned long packet)
{
    _lastPacket = packet;
}
unsigned long FilePacket::getSize()
{
    return _size;
}
std::string FilePacket::getFileName()
{
    return _fileName;
}
std::string FilePacket::getRootFolder()
{
    return _rootFolder;
}
void FilePacket::setPath(std::string folderPath)
{
    _path = folderPath;
    _path.append(_fileName);
}
boost::filesystem::path FilePacket::getPath()
{
    return _path;
}

unsigned long FilePacket::getRecovered()
{
    return static_cast<long>(_recovered.time_since_epoch().count()) / 1000000;
}

unsigned int FilePacket::getPacketsDecoded()
{
    return _packetsDecoded;
}

unsigned long FilePacket::getHeaderCreated()
{
    return _headerCreated;
}

unsigned long FilePacket::getLastDetected()
{
    return _lastDetected;
}

unsigned long FilePacket::getStartDecode()
{
    return _startDecode;
}
