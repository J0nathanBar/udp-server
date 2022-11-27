#include <iostream>
#include <boost/asio.hpp>
#include<boost/asio/io_context.hpp>
#include"UdpTX.hpp"

void waitForInput();
int main(){    
    boost::asio::io_context context;
   std::string addr= "192.168.239.216";
   int port = 42069;
   std::cout << "1" << std::endl;
   UdpTX tx(addr, port);
   std::cout<<"runnign"<<std::endl;
}
   