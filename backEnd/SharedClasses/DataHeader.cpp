#include "DataHeader.hpp"

DataHeader::DataHeader()
{
}
DataHeader::DataHeader(int blockSize, int dataSize) : _blockSize(blockSize), _dataSize(dataSize)
{
}

DataHeader::~DataHeader()
{
}