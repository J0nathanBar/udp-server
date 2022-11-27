#include<iostream>
#include "UdpReceiver.hpp"
#include <boost/asio.hpp>



int main(){
    boost::asio::io_service context;
    UdpReceiver r(42069,context);
    context.run();
}