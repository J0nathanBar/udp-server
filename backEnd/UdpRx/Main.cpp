#include <iostream>
#include "UdpReceiver.hpp"
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <mysql/mysql.h>

void init();

int main()
{

    init();


}
void init()
{
    // boost::asio::io_service context;
    boost::asio::io_context context;
    UdpReceiver r(42069, context);
    context.run();
}