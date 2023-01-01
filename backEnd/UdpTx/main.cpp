#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/filesystem.hpp>
#include "UdpTX.hpp"
#include <vector>
#include <boost/container/vector.hpp>
#include <queue>

#include <boost/algorithm/string.hpp>
#include "../SharedClasses/MyDirectory.hpp"
#include "../SharedClasses/FileManager.hpp"

void init();
int main()
{

    init();
   // boost::filesystem::path p("/home/jonny/Desktop/filestosend/");

    
}

//   File f("/home/jonny/Desktop/filestosend/duck.jpeg");
void init()
{
    boost::asio::io_context context;
    std::string addr = "192.168.47.38";
    int port = 42069;
    std::cout << "1" << std::endl;
    UdpTX tx(addr, port);
    std::cout << "runnign" << std::endl;
    tx.run();
}
