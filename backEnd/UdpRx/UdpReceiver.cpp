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

void UdpReceiver::stichFile(boost::container::map<unsigned long, FilePacket> &fileSet)
{
  std::cout << "stitch" << std::endl;
  FilePacket packet = fileSet.at(0);
  std::cout << packet.getFileData() << std::endl;
  std::string p = _currentPath;
  if (packet.getRootFolder() != ".")
    p += "/" + packet.getRootFolder();
  if (!boost::filesystem::is_directory(p))
  {
    boost::filesystem::create_directories(p);
  }
  std::cout << p << std::endl;
  packet.setPath(p);

  std::ofstream File(packet.getPath().string(), std::ios::binary);
  if (!File.is_open())
  {
    std::cout << packet.getPath().string() << std::endl;
    throw std::runtime_error("Failed to open the file");
  }

  for (unsigned long i = 0; i < fileSet.size(); i++)
  {
    // std::cout << "i: " << i << std::endl;
    packet = fileSet.at(i);
    File.write(packet.getData().data(), packet.getData().size());
  }

  File.close();
  std::cout << "File Saved: " << fileSet.at(0).getFileName() << " id: " << fileSet.at(0).getId() << std::endl;
}
void UdpReceiver::handlePacket(FilePacket fp)
{
  // std::cout << "handle packet " << std::endl;

  std::string id = fp.getId();
  auto i = _packets.find(fp.getId());
  if (i == _packets.end())
  {
    boost::container::map<unsigned long, FilePacket> fileMap;
    fileMap.emplace(fp.getIndex(), fp);
    if (fp.getLastPacket() == 1)
    {
      stichFile(fileMap);
    }
    else
      _packets.emplace(id, fileMap);
  }
  else
  {
    i->second.insert(std::make_pair(fp.getIndex(), std::move(fp)));
    //  std::cout << "set size: " << i->second.size() << " last packet -1: " << fp.getLastPacket() - 1 << std::endl;
    if (fp.getLastPacket() - 1 == i->second.size())
    {
      stichFile(i->second);
      _packets.erase(id);
    }
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
      fparse.deSerialize(hdata, header);
      _headers.push_back(std::move(header));
      _hcoder = FecCoder();
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
void UdpReceiver::stichFile(std::vector<FilePacket> &vec)
{
  FilePacket packet = vec.at(0);

  std::string p = _currentPath + packet.getRootFolder();
  if (!boost::filesystem::is_directory(p))
  {
    boost::filesystem::create_directories(p);
  }
  std::cout << p << std::endl;
  packet.setPath(p);

  std::ofstream File(packet.getPath().string(), std::ios::binary);
  if (!File.is_open())
  {
    std::cout << packet.getPath().string() << std::endl;
    throw std::runtime_error("Failed to open the file");
  }

  for (unsigned long i = 0; i < vec.size(); i++)
  {
    packet = vec.at(i);
    File.write(packet.getData().data(), packet.getData().size());
  }

  File.close();
  std::cout << "File Saved: " << vec.at(0).getFileName() << " id: " << vec.at(0).getId() << std::endl;
  vec.clear();
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
