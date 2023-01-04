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
#include <vector>
#include <boost/unordered_set.hpp>
#include <boost/container/set.hpp>
#include <unordered_set>
#include <set>
#include <functional>
#include <boost/unordered_map.hpp>
#include <boost/container/map.hpp>

enum class IPV
{
    v4,
    v6
};
class UdpReceiver
{

public:
    UdpReceiver(int port, boost::asio::io_service &context);
    ~UdpReceiver();
    void startReceive();
    void handleReceive(boost::system::error_code ec, std::size_t bytesTransferred);
    void scanConf();
    std::string stichFile(boost::container::map<unsigned long, FilePacket >  &);
    void handlePacket(std::string);
    void handleFile(std::string data);

private:
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _endpoint;
    boost::asio::io_service _context;
    boost::asio::ip::udp::endpoint _local;
    int _port;
    int k;
    boost::array<char, 8000> _buffer;
    JsonParser _jParse;
    FileParser _fParse;
    std::string _path;
    std::string _confPath;
    std::string _currentPath;
    std::thread _t;
    bool _run;
    boost::unordered_map<std::string, boost::container::map<unsigned long, FilePacket > > _packets;
};

#endif