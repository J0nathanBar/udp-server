#include <iostream>
#include "UdpReceiver.hpp"
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "File.hpp"

void init();

int main()
{

   init();

}
void init()
{
    boost::asio::io_service context;
    UdpReceiver r(42069, context);
    context.run();
}