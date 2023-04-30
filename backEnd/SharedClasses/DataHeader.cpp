#include "DataHeader.hpp"

DataHeader::DataHeader() : _empty(true), _decoded(false), _arrived(0), _firstCreated(std::chrono::system_clock::now())
{
}
DataHeader::DataHeader(unsigned int blockSize, unsigned int dataSize, std::string id, unsigned long index, std::string &hID) : _blockSize((blockSize)), _dataSize((dataSize)), _empty(false), _id(id), _index(index), _hID(hID)
{
}

DataHeader::~DataHeader()
{
}

unsigned int DataHeader::getBlockSize()
{
    return _blockSize;
}
unsigned int DataHeader::getDataSize()
{
    return _dataSize;
}

bool DataHeader::isEmpty()
{
    return _empty;
}

unsigned long DataHeader::getIndex()
{
    return _index;
}
uint8_t DataHeader::getDex()
{
    return _dex;
}
std::string DataHeader::getId()
{
    return _id;
}
void DataHeader::fill()
{
    _empty = false;
}

void DataHeader::print()
{
    std::cout << "header:: "
              << " block size: " << _blockSize << " data size: " << _dataSize << std::endl;
}

std::string DataHeader::gethID()
{
    return _hID;
}

bool DataHeader::isDecoded()
{
    return _decoded;
}

void DataHeader::setDecoded(bool decoded)
{
    _decoded = decoded;
}

unsigned int DataHeader::getArrived()
{
    return _arrived;
}



void DataHeader::arrival()
{
    _lastDetected = std::chrono::system_clock::now();
    _arrived++;
}

unsigned long DataHeader::getFirstCreated()
{
    return static_cast<long>(_firstCreated.time_since_epoch().count()) / 1000000;
}

unsigned long DataHeader::getLastDetected()
{
    return static_cast<long>(_lastDetected.time_since_epoch().count()) / 1000000;
}

void DataHeader::startDecode()
{
    _startDecode = std::chrono::system_clock::now();
}

unsigned long DataHeader::getStartDecode()
{
    return static_cast<long>(_startDecode.time_since_epoch().count()) / 1000000;
    ;
}
