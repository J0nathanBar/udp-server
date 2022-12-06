#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include "UdpTX.hpp"
#include "JsonParser.hpp"
#include <boost/filesystem.hpp>
#include "FileParser.hpp"
#include <boost/algorithm/string.hpp>
void waitForInput();
void init();
int main()
{

    init();
}

//   File f("/home/jonny/Desktop/filestosend/duck.jpeg");
void init()
{
    boost::asio::io_context context;
    std::string addr = "192.168.8.38";
    int port = 42069;
    std::cout << "1" << std::endl;
    UdpTX tx(addr, port);
    std::cout << "runnign" << std::endl;
    tx.run();
}
