#include "UdpReceiver.hpp"

UdpReceiver::UdpReceiver(int port, boost::asio::io_service &context) : _port(port),
                                                                       _socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
                                                                       _run(true)

{
  std::cout << "starting server" << std::endl;
  _t = std::thread(&UdpReceiver::scanConf, this);

  //_fileThreads = {};

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
    std::thread dataHandler(&UdpReceiver::handlePacket, this, data);
    dataHandler.detach();
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

    ModifiedFile f;
    _fParse.deSerialize(data, f);
    std::string p = _currentPath + f.getRootFolder();

    if (!boost::filesystem::is_directory(p))
    {
      boost::filesystem::create_directories(p);
    }
    f.setPath(_currentPath);
    boost::filesystem::ofstream stream{*f.getPath()};
    stream << f.getData();
  }
  catch (boost::filesystem::filesystem_error &e)
  {
    std::cerr << e.what() << std::endl;
  }
}
std::string UdpReceiver::stichFile(boost::container::map<unsigned long, FilePacket> &fileSet)
{
  std::string stichedData = "";

  for (unsigned long i = 0; i < fileSet.size(); i++)
  {

    stichedData += fileSet.at(i).getData();
  }
  return stichedData;
}
void UdpReceiver::handlePacket(std::string data)
{
  FilePacket fp;
  _fParse.deSerialize(data, fp);
  std::string id = fp.getId();
  auto i = _packets.find(id);

  if (i == _packets.end())
  {
    boost::container::map<unsigned long, FilePacket> fileMap;
    fileMap.emplace(fp.getIndex(), fp);
    if (fp.getLastPacket() == 1)
    {
      handleFile(stichFile(fileMap));
    }
    else
      _packets.emplace(id, fileMap);
  }
  else
  {
    i->second.insert(std::make_pair(fp.getIndex(), std::move(fp)));
    if (fp.getLastPacket() == i->second.size())
    {
      handleFile(stichFile(i->second));
      _packets.erase(id);
    }
  }
  // int i;
}