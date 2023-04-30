
#ifndef DATAHEADER_H
#define DATAHEADER_H

#pragma once
#include <boost/serialization/serialization.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <boost/format.hpp>
#include <chrono>

class DataHeader
{
public:
    DataHeader();
    DataHeader(unsigned int blockSize, unsigned int dataSize, std::string id, unsigned long index, std::string &hID);
    ~DataHeader();
    unsigned int getBlockSize();
    unsigned int getDataSize();
    unsigned long getIndex();
    uint8_t getDex();
    std::string getId();
    bool isEmpty();
    void fill();
    void print();
    std::string gethID();
    bool isDecoded();
    void setDecoded(bool decoded);
    unsigned int getArrived();
    void arrival();
    unsigned long getFirstCreated();
    unsigned long getLastDetected();
    void startDecode();
    unsigned long getStartDecode();

private:
    unsigned int _blockSize, _dataSize;
    unsigned long _index;
    unsigned int _arrived, _decoded;
    uint8_t _dex;
    std::string _id, _hID;
    bool _empty;
   // bool _decoded;
    std::chrono::high_resolution_clock::time_point _firstCreated, _lastDetected, _startDecode;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &_hID;
        ar &_blockSize;
        ar &_dataSize;
        ar &_id;
        ar &_index;
        ar &_dex;
    }
};

#endif
