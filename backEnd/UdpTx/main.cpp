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
#include <unordered_set>

void init();
int main()
{
   // std::cout << "eee"<< std::endl;
   // init();
    
    boost::filesystem::path p("/home/jonny/Desktop/filestosend/duck.jpeg");
    ModifiedFile f(p);
    std::cout  << f.getId() <<std::endl;
  /*  FileParser fp;
    std::string  parsedData = fp.serialize(f);
    int packetSize=3000;
    std::cout << parsedData.length() << std::endl;
    //std::cout << parsedData << '\n' << '\n' << '\n' << '\n' << "---------------------"<< std::endl;
    std::queue<std::string> q;
    for (unsigned long i = 0; i < parsedData.length(); i+=packetSize)
    {
        std::string splicedData = parsedData.substr(i,packetSize);
        q.emplace(splicedData);
        
    }
    std::string stichedData ="";
    while(!q.empty()){
        stichedData += q.front();
        q.pop();
    }
    std::cout << stichedData.compare(parsedData) << std::endl;*/
   
    

  


    
}

//   File f("/home/jonny/Desktop/filestosend/duck.jpeg");
void init()
{
    boost::asio::io_context context;
    std::string addr = "10.0.0.16";
    int port = 42069;
    std::cout << "1" << std::endl;
    UdpTX tx(addr, port);
    std::cout << "runnign" << std::endl;
    tx.run();
}
