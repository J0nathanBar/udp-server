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
    // std::vector a = {1, 2, 3, 4, 5, 6};
    // std::cout << a.capacity() << std::endl;
    // std::vector b = std::move(a);
    // std::cout << a.capacity() << std::endl;
}

//   File f("/home/jonny/Desktop/filestosend/duck.jpeg");
void init()
{
    std::cout << "init " << std::endl;
    boost::asio::io_context context;
    std::string addr = "127.0.0.1";
    int port = 42069;
    UdpTX tx(addr, port);
    tx.run();
}
