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

    //changed ports might do problems mush check
    UdpReceiver r(42068, context);
    //check the ports!!!!!
    
    context.run();
}