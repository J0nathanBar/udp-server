#include "DataHeader.hpp"

DataHeader::DataHeader() : _empty(true)
{
}
DataHeader::DataHeader(unsigned int blockSize, unsigned int dataSize) : _blockSize((blockSize)), _dataSize((dataSize)), _empty(false)
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

void DataHeader::fill()
{
    _empty = false;
}
