#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include "UdpTX.hpp"
#include "../SharedClasses/FecCoder.hpp"
#include "../SharedClasses/FileParser.hpp"

void init();

int main()
{
    //  init();
    // boost::filesystem::path p("/home/jonny/Desktop/filestosend/duck.jpeg");
    // ModifiedFile f(p);

   // FecCoder coder;
    //  std::vector<int> v = {1,2,3,4,5};
    // boost::filesystem::path p("/home/jonny/Desktop/filestosend/duck.jpeg");
    // ModifiedFile f(p);
    // std::string a("i enjoy eating a lot of stuff");
    // auto v = coder.encode(a, 28);
    // for (int i = 0; i < v->size(); i++)
    // {
    //     std::vector<uint8_t> b = v->at(i);
    //     coder.decode(b, a.length());
    // }
    // std::string a(coder.ddd);
    // boost::filesystem::path r("/home/jonny/Desktop/duck.jpeg");
    // boost::filesystem::ofstream stream(r);
    // stream << a;
    DataHeader h(200, 700);
    FileParser fp;
    std::string a = fp.serialize(h);
    std::cout << "size of 2 integers: " << sizeof(int) * 2 << std::endl;
    std::cout << "size of header: " << sizeof(DataHeader) << std::endl;
    std::cout << "size of defined header: " << sizeof(h) << std::endl;
    std::cout << "size of serilized header: " << sizeof(a) << std::endl;
    std::cout << "length of serilized header: " << a.length() << std::endl;
    h = DataHeader(7255, 5);
    a = fp.serialize(h);
    std::cout << "changing values... \n\n\n"
              << std::endl;
    std::cout << "size of defined header: " << sizeof(h) << std::endl;
    std::cout << "size of serilized header: " << sizeof(a) << std::endl;
    std::cout << "length of serilized header: " << a.length() << std::endl;
    init();
}

//   File f("/home/jonny/Desktop/filestosend/duck.jpeg");
void init()
{
    boost::asio::io_context context;
    std::string addr = "192.168.93.38";
    int port = 42069;
    UdpTX tx(addr, port);
    tx.run();
}
