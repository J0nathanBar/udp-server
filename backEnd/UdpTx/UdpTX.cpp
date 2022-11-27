#include "UdpTX.hpp"

UdpTX::UdpTX(std::string addr, int port) :
 _resolver(_context),
 _socket(_context),
 _endpoint(boost::asio::ip::make_address(addr), port)
{
  _socket.open(udp::v4());
  writeData();
}

UdpTX::~UdpTX()
{
}

void UdpTX::writeData()
{
  std::string msg = " ";
  while (msg != "")
  {
    std::cout << "type message" << std::endl;
    getline(std::cin, msg);
    if (msg != "")
    {
      sendData(msg);
    }
  }
}

void UdpTX::sendData(std::string msg)
{
  boost::array<std::string, 1000> send_buf = {msg};
  _socket.send_to(boost::asio::buffer(msg), _endpoint);
}

void UdpTX::write()
{
  while (true)
  {
    std::string msg;
    std::cout << "type message" << std::endl;
    getline(std::cin, msg);
    if (msg != "")
    {
      post(msg);
    }
  }
}
void UdpTX::post(const std::string &message)
{
  bool queueIdle = _buf.empty();
  _buf.push(message);
  if (queueIdle)
    asyncSend();
}

void UdpTX::asyncSend()
{
  _socket.async_send_to(boost::asio::buffer(_buf.front()), 
  _endpoint, 
  [this](boost::system::error_code ec, std::size_t bytesTransferred){ 
    handleSend(ec, bytesTransferred); });
}

void UdpTX::handleSend(const boost::system::error_code ec, std::size_t bytesTransferred)
{
  std::cout << "bytes transferred: " << bytesTransferred << std::endl;
  _buf.pop();
  if (!_buf.empty())
  {
    asyncSend();
  }
}
void UdpTX::run()
{
  _socket.open(udp::v4());
  _context.run();
}