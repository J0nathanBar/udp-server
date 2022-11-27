#include "UdpReceiver.hpp"

UdpReceiver::UdpReceiver(int port,boost::asio::io_service& context):
_port(port),
_socket(context,boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
{
  std::cout<<"starting server" <<std::endl;
  startReceive();
}
UdpReceiver::~UdpReceiver(){
}
void UdpReceiver::startReceive(){
  std::cout << "recv" << std::endl;
  _socket.async_receive_from(boost::asio::buffer(_buffer),
     _endpoint,[this](boost::system::error_code  ec,std::size_t bytesTransferred){
    handleReceive(ec,bytesTransferred); 
    });
}
void UdpReceiver::handleReceive(boost::system::error_code  ec,std::size_t bytesTransferred ){
  std::cout <<"bytes transferred: "<<bytesTransferred<<std::endl;
  if(!ec){
    std::string data;
    std::copy(_buffer.begin(), _buffer.begin()+bytesTransferred, std::back_inserter(data));
    std::cout << data<< std::endl;
    }
  startReceive();
}