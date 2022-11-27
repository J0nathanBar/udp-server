#ifndef UDPTX_HPP
#define UDPTX_HPP


#pragma once
#include <boost/asio.hpp>
#include<boost/array.hpp>
#include<boost/bind/bind.hpp>
#include<iostream>
#include<queue>
#include<string>
#include<thread>

using boost::asio::ip::udp;

class UdpTX
{
public:
    UdpTX(std::string addr, int port);
    ~UdpTX();
    void writeData();
    void sendData(std::string);
    void handleSend(const boost::system::error_code ,std::size_t);
    void write();
    void post(const std::string& message);
    void start();
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
//boost::array<char, 1> send_buf  = {{ 0 }};
 //std::queue<std::string> _outgoingMessages{};
std::queue<std::string> _buf{};
};

#endif