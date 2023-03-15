#include "UdpReceiver.hpp"

UdpReceiver::UdpReceiver(int port, boost::asio::io_service &context) : _port(port),
                                                                       _socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
                                                                       _run(true), _confPath("/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/RecvConf.json"), hcounter(0), hSize(0)

{
  std::cout << "starting server" << std::endl;
  _t = std::thread(&UdpReceiver::scanConf, this);
  _buffer.resize(SIZE);
  packetCounter = 0;
  _coders.push_back(FecCoder());
  startReceive();
}
UdpReceiver::~UdpReceiver()
{
  _t.detach();
  for (int i = 0; i < _threads.size(); i++)
  {
    _threads.at(i).join();
  }
}
void UdpReceiver::startReceive()
{
  try
  {
    _socket.async_receive_from(boost::asio::buffer(_buffer),
                               _endpoint, [this](boost::system::error_code ec, std::size_t bytesTransferred)
                               { handleReceive(ec, bytesTransferred); });
  }
  catch (std::exception &ex)
  {
    std::cout << "recieve error: " << ex.what() << " " << _buffer.capacity() << std::endl;
  }
}
void UdpReceiver::handleReceive(boost::system::error_code ec, std::size_t bytesTransferred)
{
  std::vector<uint8_t> v(bytesTransferred);
  std::copy(_buffer.begin(), _buffer.begin() + bytesTransferred, v.begin());
  startReceive();
  try
  {
    if (bytesTransferred == 1 && !ec)
    {
      if (hSize == 0)
        std::cout << "full loop" << std::endl;
      _threads.push_back(std::thread(&UdpReceiver::processData, this, std::move(_v), hSize++));
    }

    else if (!ec && bytesTransferred > 0)
    {

      int id = v.at(--bytesTransferred);
      v.erase(v.begin() + bytesTransferred);
      if (id == 69 /*&& _header.isEmpty()*/)
      {

        handleHeader(bytesTransferred, v);
        // _threads.push_back(std::thread(&UdpReceiver::handleHeader, this, bytesTransferred, std::move(v)));
      }
      else if (id == 0 /*&& !_header.isEmpty()*/)
      {
        int k = hSize - 1;
        _v.push_back(std::move(v));
        // _threads.push_back(std::thread(&UdpReceiver::handleRawData, this, std::move(v), k, packetCounter));
      }
    }

    // packetCounter++;
  }
  catch (std::exception &ex)
  {
    std::cout << "recieve error: " << ex.what() << " " << _buffer.capacity() << std::endl;
  }
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
        std::cout << "c path: " << _currentPath << std::endl;
      }
    }
    catch (nlohmann::json::parse_error &ex)
    {
      std::cerr << ex.what() << std::endl;
    }
  }
}

void UdpReceiver::handlePacket(FilePacket fp)
{
  try
  {
    std::unique_lock lock(_fileMutex);
    std::string id = fp.getId();
    auto i = _files.find(fp.getId());
    if (i == _files.end())
    {
      std::string p = _currentPath;
      if (fp.getRootFolder() != ".")
        p += "/" + fp.getRootFolder();
      fp.setPath(p);
      if (!boost::filesystem::is_directory(p))
      {
        boost::filesystem::create_directories(p);
      }
      _files.emplace(std::move(id), ModifiedFile(std::move(fp)));
    }
    else
    {
      i->second.appendPacket(std::move(fp));
    }
    lock.unlock();
  }
  catch (const std::exception &e)
  {
    std::cerr << "handle packet: " << e.what() << std::endl;
  }
}

void UdpReceiver::handleHeader(int bytesTransferred, std::vector<uint8_t> buffer)
{
  try
  {
    std::unique_lock lock(_headerMutex);
    std::string hdata(_hcoder.decodeHeader(buffer, bytesTransferred));
    if (hdata != "")
    {
      FileParser fparse;
      DataHeader header;

      if (fparse.deSerialize(hdata, header))
      {
        _headers.push_back(std::move(header));
        _hcoder = FecCoder();
      }
      else
      {
        std::cout << "header failed wtf is going on" << std::endl;
        std::cout << "bytes Transferred" << bytesTransferred << std::endl;
      }
    }
    lock.unlock();
  }
  catch (std::exception &e)
  {
    std::cout << "handle Header Excpetion: " << e.what() << std::endl;
  }
}
void UdpReceiver::handleRawData(std::vector<uint8_t> buffer, int id, int counter, int dataSize, int blockSize)
{
  try
  {

    std::string data;
    std::unique_lock lock(_coderMutex);
    data = _coders.at(id).decode(buffer, dataSize, blockSize);
    lock.unlock();

    if (data == ".")
    {
      std::cout << "error at: " << id << std::endl;
      std::cout << "data size" << dataSize << " blockSize: " << blockSize << std::endl;
      std::unique_lock lockk(_headerMutex);
      _headers.at(id).print();
      lockk.unlock();
    }
    else if (data != "")
    {
      FilePacket fp;
      FileParser fparse;
      if (fparse.deSerialize(data, fp))
        handlePacket(std::move(fp));
      else
      {
        std::cout << "id: " << id << " index: " << counter << std::endl;
      }
    }
  }
  catch (std::exception &e)
  {
    std::unique_lock lock(_coderMutex);
    std::cout << "handle raw Excpetion: " << e.what() << std::endl;
    std::cout << "coder size" << _coders.size() << std::endl;
    lock.unlock();
  }
}

void UdpReceiver::processData(std::vector<std::vector<uint8_t>> v, int id)
{
  std::unique_lock lockk(_headerMutex);
  bool locked = true;
  while (id > _headers.size() - 1)
  {
    if (locked)
    {
      locked = false;
      lockk.unlock();
    }
  }
  if (!locked)
    lockk.lock();
  int dataSize = _headers.at(id).getDataSize();
  int blockSize = _headers.at(id).getBlockSize();
  lockk.unlock();
  std::unique_lock lock(_coderMutex);
  while (id > _coders.size())
  {
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    lock.lock();
  }
  if (id == _coders.size())
  {

    _coders.push_back(FecCoder());
  }
  lock.unlock();
  for (int i = 0; i < v.size(); i++)
  {
    handleRawData(v.at(i), id, i, dataSize, blockSize);
  }
}
