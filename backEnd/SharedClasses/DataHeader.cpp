#include "DataHeader.hpp"

DataHeader::DataHeader() : _empty(true)
{
}
DataHeader::DataHeader(unsigned int blockSize, unsigned int dataSize, std::string id, unsigned long index) : _blockSize((blockSize)), _dataSize((dataSize)), _empty(false), _id(id), _index(index)
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

unsigned long DataHeader::getIndex(){
    return _index;
}
std::string DataHeader::getId(){
    return _id;
}
void DataHeader::fill()
{
    _empty = false;
}


void DataHeader::print()
{
    //   std::cout << " block size: " << _blockSize << " data size: " << _dataSize << std::endl;
}
