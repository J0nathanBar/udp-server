#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include "UdpTX.hpp"
#include "../SharedClasses/FecCoder.hpp"
#include "../SharedClasses/FileParser.hpp"
#include <codecvt>
#include <locale>
#include <string>

void init();

int main()
{
    init();
}

//   File f("/home/jonny/Desktop/filestosend/duck.jpeg");
void init()
{
    boost::asio::io_context context;
    std::string addr = "127.0.0.1";
    int port = 42069;
    UdpTX tx(addr, port);
    tx.run();
}
