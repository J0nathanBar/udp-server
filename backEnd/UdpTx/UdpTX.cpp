#include "UdpTX.hpp"

UdpTX::UdpTX(std::string addr, int port) : _resolver(_context), _socket(_context),
                                           _endpoint(boost::asio::ip::make_address(addr), port),
                                           _run(false), _manager(_buf, _bufferMutex)
{
  packetCounter = 0;
}

UdpTX::~UdpTX()
{
}

void UdpTX::asyncSend()
{
  while (_buf.empty())
  {
    // std::cout << "empty" << std::endl;
  }
  std::lock_guard<std::mutex> guard(_bufferMutex);
  _socket.async_send_to(boost::asio::buffer(_buf.front()), _endpoint,
                        [this](boost::system::error_code ec, std::size_t bytesTransferred)
                        { handleSend(ec, bytesTransferred); });
}

void UdpTX::handleSend(const boost::system::error_code ec, std::size_t bytesTransferred)
{

  std::cout << "bytes transferred: " << bytesTransferred << " packet number: " << packetCounter << std::endl;
  _buf.pop();
  packetCounter++;
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  asyncSend();
}
void UdpTX::run()
{
  _socket.open(udp::v4());
  _manager.start();
  _run = true;
  asyncSend();
  _context.run();
}
