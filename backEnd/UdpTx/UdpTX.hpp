#ifndef UDPTX_HPP
#define UDPTX_HPP

#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include<mutex>
#include "../SharedClasses/FileParser.hpp"
#include "../SharedClasses/json.hpp"
#include "../SharedClasses/JsonParser.hpp"
#include "../SharedClasses/MyDirectory.hpp"
#include "../SharedClasses/FileManager.hpp"
#include <wirehair/wirehair.h>
#include <chrono>

using boost::asio::ip::udp;

class UdpTX
{
public:
    UdpTX(std::string addr, int port);
    ~UdpTX();
    void handleSend(const boost::system::error_code, std::size_t);
    void asyncSend();
    void run();

private:
    boost::asio::io_context _context;
    udp::resolver _resolver;
    udp::endpoint _endpoint;
    udp::socket _socket;
    std::string _addr;
    std::thread t;
    int _port;
    std::queue<std::vector<uint8_t>> _buf{};
    bool _run;
    FileManager _manager;
    int packetCounter;
    std::mutex _bufferMutex;
};

#endif