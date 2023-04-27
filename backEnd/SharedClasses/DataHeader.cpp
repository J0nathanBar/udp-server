#include "DataHeader.hpp"

DataHeader::DataHeader() : _empty(true), _decoded(false), _arrived(0), _afterDecode(0)
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

unsigned long DataHeader::getArrived()
{
    return _arrived;
}

unsigned long DataHeader::getAlreadyDecoded()
{
    return _afterDecode;
}

void DataHeader::arrival()
{
    _arrived++;
    if (_decoded)
        _afterDecode++;
}
