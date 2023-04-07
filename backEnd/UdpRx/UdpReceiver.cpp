#include "UdpReceiver.hpp"

UdpReceiver::UdpReceiver(int port, boost::asio::io_service &context) : _port(port),
                                                                       _socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
                                                                       _run(true), _confPath("/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/RecvConf.json"), hcounter(0), hSize(0)

{
  std::cout << "starting server" << std::endl;
  _t = std::thread(&UdpReceiver::scanConf, this);
  _buffer.resize(SIZE);
  packetCounter = 0;
  // _coders.push_back(FecCoder());
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
  std::cout << "counter: " << packetCounter++ << std::endl;
  std::vector<uint8_t> v(bytesTransferred);
  std::copy(_buffer.begin(), _buffer.begin() + bytesTransferred, v.begin());
  startReceive();
  std::string hID;
  try
  {
    if (!ec && bytesTransferred > 0)
    {
      int id = v.at(--bytesTransferred);
      v.erase(v.begin() + bytesTransferred);
      if (id == 69)
      {
        handleHeader(bytesTransferred, v);
      }
      else if (id == 0)
      {
        std::vector<uint8_t> hBytes(v.end() - 8, v.end());
        hID = std::string(hBytes.begin(), hBytes.end());
        v.erase(v.end() - 8, v.end());
        std::unique_lock lock(cMutex);
        if (counters.find(hID) == counters.end())
        {
          counters.emplace(hID, 1);
        }
        else
          counters.at(hID)++;
        // std::cout << "coming at: " << hID << " number " << counters.at(hID) << std::endl;
        lock.unlock();
        int k = handleRawData(std::move(v), hID);
        if (k == 2 && counters.at(hID) == 20)
          std::cout << "bruh" << std::endl;
      }
    }

    // packetCounter++;
  }
  catch (std::exception &ex)
  {
    std::cout << "handle recieve error: " << ex.what() << std::endl;
    //   std::cout << "at: " << hID << /*" number " << counters.at(hID) <<*/ std::endl;
    return;
  }
}

void UdpReceiver::scanConf()
{
  while (_run)
  {
    try
    {
      // _path = _jParse.parse(_confPath);
      int z = k = 0;
      _jParse.convertJson(false, _confPath, z, k, _path);
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
        _headers.emplace(header.gethID(), std::move(header));
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
int UdpReceiver::handleRawData(std::vector<uint8_t> buffer, std::string id) // 0 = success, 1 = fail, 2 = awaiting for more data
{
  try
  {
    //  std::cout << "handle raw" << std::endl;
    std::unique_lock hlock(_headerMutex);

    while (_headers.find(id) == _headers.end())
    {
      hlock.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      hlock.lock();
    }

    int dataSize = _headers.at(id).getDataSize();
    int blockSize = _headers.at(id).getBlockSize();
    hlock.unlock();
    std::string data;
    std::unique_lock lock(_coderMutex);
    if (_coders.find(id) == _coders.end())
      _coders.emplace(id, FecCoder());
    data = _coders.at(id).decode(buffer, dataSize, blockSize);
    lock.unlock();

    if (data == ".")
    {
      std::cout << "error at: " << id << std::endl;
      std::cout << "data size" << dataSize << " blockSize: " << blockSize << std::endl;
      std::unique_lock lockk(_headerMutex);
      _headers.at(id).print();
      lockk.unlock();
      return 1;
    }
    // else if (data == "" && counter == numOfBlocks - 1)
    // {
    //   std::cout << "data wasnt sufficient" << std::endl;
    //   std::cout << " data size: " << dataSize << " blockSize: " << blockSize << std::endl;
    //   return 1;
    // }
    else if (data != "")
    {
      FilePacket fp;
      FileParser fparse;
      if (fparse.deSerialize(data, fp))
      {
        handlePacket(std::move(fp));
        return 0;
      }
      else
      {
        //   std::cout << "id: " << id << " index: " << counter << std::endl;
        return 1;
      }
    }
  }
  catch (std::exception &e)
  {
    std::cout << "handle raw Excpetion: " << e.what() << std::endl;
  }
  return 2;
}

void UdpReceiver::processData(std::vector<std::vector<uint8_t>> v, int id)
{
  // std::unique_lock lockk(_headerMutex);
  // bool locked = true;
  // while (id > _headers.size() - 1)
  // {
  //   if (locked)
  //   {
  //     locked = false;
  //     lockk.unlock();
  //   }
  // }
  // if (!locked)
  //   lockk.lock();
  // int dataSize = _headers.at(id).getDataSize();
  // int blockSize = _headers.at(id).getBlockSize();
  // lockk.unlock();
  // std::unique_lock lock(_coderMutex);
  // while (id > _coders.size())
  // {
  //   lock.unlock();
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1));
  //   lock.lock();
  // }
  // if (id == _coders.size())
  // {

  //   _coders.push_back(FecCoder());
  // }
  // lock.unlock();
  // for (int i = 0; i < v.size(); i++)
  // {
  //   std::vector<uint8_t> hBytes(v.at(i).end() - 8, v.at(i).end());
  //   std::string hID(hBytes.begin(), hBytes.end());
  //   v.at(i).erase(v.at(i).end() - 8, v.at(i).end());
  //   if (hID != _headers.at(id).gethID())
  //   {
  //     std::cout << "sneaky packet " << std::endl;
  //     continue;
  //   }

  //   int res = handleRawData(v.at(i), id, i, dataSize, blockSize, v.size());
  //   if (res == 0)
  //     break;
  //   if (res == 1)
  //   {
  //     std::cout << " vector's size: " << v.size() << std::endl;
  //   }
  // }
}