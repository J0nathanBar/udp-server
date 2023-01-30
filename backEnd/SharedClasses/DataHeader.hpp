#ifndef DATAHEADER_H
#define DATAHEADER_H

#pragma once
#include <boost/serialization/serialization.hpp>

class DataHeader
{
public:
    DataHeader();
    DataHeader(int blockSize, int dataSize);
    ~DataHeader();

private:
    int _blockSize;
    int _dataSize;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &_blockSize;
        ar &_dataSize;
    }
};

#endif