#ifndef UDPRECEIVER_HPP
#define UDPRECEIVER_HPP

#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include "../SharedClasses/FileParser.hpp"
#include "../SharedClasses/JsonParser.hpp"
#include "../SharedClasses/FilePacket.hpp"
#include "../SharedClasses/ModifiedFile.hpp"
#include <vector>
#include <boost/unordered_set.hpp>
#include <boost/container/set.hpp>
#include <unordered_set>
#include <set>
#include <functional>
#include <boost/unordered_map.hpp>
#include <boost/container/map.hpp>
#include <memory>
#include <mutex>
#include "../SharedClasses/FecCoder.hpp"

enum class IPV
{
    v4,
    v6
};
class UdpReceiver
{

public:
    UdpReceiver(int port, boost::asio::io_context &context);
    ~UdpReceiver();
    void startReceive();
    void handleReceive(boost::system::error_code ec, std::size_t bytesTransferred);
    void scanConf();
    void handlePacket(FilePacket fp);
    void handleHeader(int, std::vector<uint8_t> buffer);
    int handleRawData(std::vector<uint8_t> buffer, std::string headerId);

private:
    static constexpr int SIZE = 10000;

    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _endpoint;
    boost::asio::io_context &_context;
    boost::asio::ip::udp::endpoint _local;
    int _port;
    int k;
    std::vector<uint8_t> _buffer;
    std::vector<std::vector<uint8_t>> _v;
    JsonParser _jParse;
    FileParser _fParse;
    std::string _path;
    std::string _currentPath;
    std::thread _t, cleaner;
    std::vector<std::thread> _threads;
    bool _run;
    std::mutex _coderMutex, _headerMutex, _fileMutex, _threadMutex, cMutex;

    boost::unordered_map<std::string, ModifiedFile> _files;
    FecCoder _hcoder;
    int eff = 0;
    boost::unordered_map<std::string, DataHeader> _headers;
    boost::unordered_map<std::string, FecCoder> _coders;

    double max = 0;
    double avg = 0;
    // boost::asio::io_context _context;
    int hSize;
    int hcounter;
    int packetCounter;
};

#endif