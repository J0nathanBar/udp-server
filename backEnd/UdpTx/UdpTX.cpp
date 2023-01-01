#include "UdpTX.hpp"

UdpTX::UdpTX(std::string addr, int port) : _resolver(_context), _socket(_context),
                                           _endpoint(boost::asio::ip::make_address(addr), port),
                                           _run(false), _manager(_buf)
{
  //_socket.open(udp::v4());
}

UdpTX::~UdpTX()
{
}

void UdpTX::sendData(std::string msg)
{
  boost::array<std::string, 1000> send_buf = {msg};

  _socket.send_to(boost::asio::buffer(msg), _endpoint);
  std::cout << "data sent" << std::endl;
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
  while (_buf.empty())
  {
  }
  _socket.async_send_to(boost::asio::buffer(_buf.front()), _endpoint,
                        [this](boost::system::error_code ec, std::size_t bytesTransferred)
                        { handleSend(ec, bytesTransferred); });
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
  _manager.start();
  _run = true;
  asyncSend();
  _context.run();
}