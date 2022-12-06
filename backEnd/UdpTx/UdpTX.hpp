#ifndef UDPTX_HPP
#define UDPTX_HPP

#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include<boost/algorithm/string.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <thread>

#include "json.hpp"
#include "FileParser.hpp"
#include "JsonParser.hpp"

using boost::asio::ip::udp;

class UdpTX
{
public:
    UdpTX(std::string addr, int port);
    ~UdpTX();
    void sendData(std::string);
    void handleSend(const boost::system::error_code, std::size_t);
    void post(const std::string &message);
    void start();
    void asyncSend();
    void run();
    void scanConf();
    void handleFile();

private:
    boost::asio::io_context _context;
    udp::resolver _resolver;
    udp::endpoint _endpoint;
    udp::socket _socket;
    std::string _addr;
    std::thread t;
    int _port;
    std::queue<std::string> _buf{};
    JsonParser _jParse;
    FileParser _fParse;
    bool _run;
    std::string _path;
    std::string _currentPath;
    std::string _confPath;
};

#endif