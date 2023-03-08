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
    DataHeader(unsigned int blockSize, unsigned int dataSize, std::string id, unsigned long index);
    void toLong();
    ~DataHeader();
    unsigned int getBlockSize();
    unsigned int getDataSize();
    unsigned long getIndex();
    uint8_t getDex();
    std::string getId();
    bool isEmpty();
    void fill();
    void print();

private:
    unsigned int _blockSize, _dataSize;
    unsigned long _index;
    uint8_t _dex;
    std::string _id;
    bool _empty;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &_blockSize;
        ar &_dataSize;
        ar &_id;
        ar &_index;
        ar & _dex;
    }
};

#endif