#ifndef FECCODER_HPP
#define FECCODER_HPP

#pragma once
#include <wirehair/wirehair.h>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include<queue>
#include "DataHeader.hpp"
#include "FileParser.hpp"
class FecCoder
{
public:
    FecCoder();
    ~FecCoder();
    bool recover();
    std::shared_ptr<std::queue<std::vector<uint8_t>>> encode(const std::string &data, const int kPacketSize);
    std::shared_ptr<std::queue<std::vector<uint8_t>>> encode(const std::vector<uint8_t> &data, const int kPacketSize);
    bool decode(std::vector<uint8_t> &block, int kMessageBytes);
    std::string getRecovered();
    void makeHeader(std::shared_ptr<std::queue<std::vector<uint8_t>>>,int kPacketSize,int kMessageBytes);

private:
    WirehairCodec _encoder, _decoder;
    std::vector<uint8_t> _decoded;
    std::string _data;
    unsigned int _blockId;
    bool rec;
    const int _headerLength = 45;
    int _lastPacketSize;
};

#endif