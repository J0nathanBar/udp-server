#ifndef DATAHEADER_H
#define DATAHEADER_H

#pragma once
#include <boost/serialization/serialization.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <boost/format.hpp>

class DataHeader
{
public:
    DataHeader();
    DataHeader(unsigned int blockSize, unsigned int dataSize);
    void toLong();
    ~DataHeader();
    unsigned int  getBlockSize();
    unsigned int getDataSize();
    bool isEmpty();
    void fill();


private:
   


    unsigned int _blockSize, _dataSize;
    bool _empty;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &_blockSize;
        ar &_dataSize;
    }
};

#endif