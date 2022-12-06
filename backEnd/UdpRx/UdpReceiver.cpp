#include "UdpReceiver.hpp"

UdpReceiver::UdpReceiver(int port, boost::asio::io_service &context) : _port(port),
                                                                       _socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
                                                                       _run(true),
                                                                       _confPath("/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/RecvConf.json"),
                                                                       _path(""), _currentPath("")
{
  std::cout << "starting server" << std::endl;
  _t = std::thread(&UdpReceiver::scanConf, this);

  startReceive();
}
UdpReceiver::~UdpReceiver()
{
    _t.detach();
}
void UdpReceiver::startReceive()
{
  std::cout << "recv" << std::endl;
  _socket.async_receive_from(boost::asio::buffer(_buffer),
                             _endpoint, [this](boost::system::error_code ec, std::size_t bytesTransferred)
                             { handleReceive(ec, bytesTransferred); });
}
void UdpReceiver::handleReceive(boost::system::error_code ec, std::size_t bytesTransferred)
{
  std::cout << "bytes transferred: " << bytesTransferred << std::endl;
  if (!ec)
  {
    std::string data;
    std::copy(_buffer.begin(), _buffer.begin() + bytesTransferred, std::back_inserter(data));
    std::cout << data << std::endl;
    std::thread dataHandler(&UdpReceiver::handleFile, this, data);
  }
  startReceive();
}
void UdpReceiver::scanConf()
{
  while (_run)
  {
    try
    {
      _path = _jParse.parse(_confPath);
      boost::erase_all(_path, "\"");
      if (_path.compare(_currentPath) != 0)
      {
        _currentPath = _path;
        std::cout << _currentPath << std::endl;
      }
    }
    catch (nlohmann::json::parse_error &ex)
    {
      std::cerr << ex.what() << std::endl;
    }
  }
}

void UdpReceiver::handleFile(std::string data)
{
  try
  {
    File f;
    _fParse.deSerialize(data, &f);
    f.setPath(_currentPath);
    boost::filesystem::ofstream stream{*f.getPath()};
    stream << f.getData();
  }
  catch (boost::filesystem::filesystem_error &e)
  {
    std::cerr << e.what() << std::endl;
  }
}