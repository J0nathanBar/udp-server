#include "UdpTX.hpp"

UdpTX::UdpTX(std::string addr, int port) : _resolver(_context),
_socket(_context),
_endpoint(boost::asio::ip::make_address(addr), port),
_run(false),
_path(""),
_currentPath(""),
_confPath("/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/TransConf.json")
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
  _context.run();
  _run = true;
  scanConf();
}
void UdpTX::scanConf()
{
  while (_run)
  {
    try
    {
      _path = _jParse.parse(_confPath);
      boost::erase_all(_path,"\"");
      if (_path.compare(_currentPath) != 0)
      {
        _currentPath = _path;
        std::cout << _currentPath << std::endl;
        handleFile();
      }
    }
    catch (nlohmann::json::parse_error &ex)
    {
      std::cerr<<ex.what()<<std::endl;
    }
  }
}
void UdpTX::handleFile(){
  try
  {
    ModifiedFile f(_currentPath);
    std::string parsedFile = _fParse.serialize(&f);
    sendData(parsedFile);
    
  }
  catch(boost::filesystem::filesystem_error& e)
  {
    std::cerr << e.what() << std::endl;
  }
  

}