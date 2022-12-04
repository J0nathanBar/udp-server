#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include "UdpTX.hpp"
#include "JsonParser.hpp"
#include <boost/filesystem.hpp>
#include "FileParser.hpp"
void waitForInput();
void init();
int main()
{
    File f("/home/jonny/Desktop/filestosend/duck.jpeg");
    FileParser fp;
    std::string data;
    data = fp.serialize(&f);
    File d;
    fp.deSerialize(data, &d);
    std::cout << d.getFileName() << std::endl;
}

void init()
{
    boost::asio::io_context context;
    std::string addr = "192.168.239.216";
    int port = 42069;
    std::cout << "1" << std::endl;
    UdpTX tx(addr, port);
    std::cout << "runnign" << std::endl;
    JsonParser j;
    tx.sendData(j.parse(""));
}
