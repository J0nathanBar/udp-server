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
  try
  {

    // // std::cout << "recv: " << packetCounter << std::endl;

    // std::cout << "---------------------------------------------------" << std::endl;
    // std::cout << "bytes transferred: " << bytesTransferred << std::endl;
    if (bytesTransferred == 1 && !ec)
    {
      hSize++;
      _threads.push_back(std::thread(&UdpReceiver::processData, this, std::move(_v)));
    }

    else if (!ec && bytesTransferred > 0)
    {
      std::vector<uint8_t> v(bytesTransferred);
      std::copy(_buffer.begin(), _buffer.begin() + bytesTransferred, v.begin());

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

    packetCounter++;
    startReceive();
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

  for (unsigned long i = 0; i < fileSet.size(); i++)
  {
   // std::cout << "i: " << i << std::endl;
    packet = fileSet.at(i);
    File.write(packet.getData().data(), packet.getData().size());
  }

  File.close();
  std::cout << "File Saved: " << fileSet.at(0).getFileName() << " id: " << fileSet.at(0).getId() << std::endl;
}
void UdpReceiver::handlePacket(std::string data)
{
  // std::cout << "handle packet " << std::endl;
  FilePacket fp;
  FileParser fparse;
  fparse.deSerialize(data, fp);

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
void UdpReceiver::handleRawData(std::vector<uint8_t> buffer, int id, int counter)
{
  try
  {

    std::string data;

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
    std::unique_lock lock(_coderMutex);
    if (id > _coders.size() - 1)
    {

      _coders.push_back(FecCoder());
    }
    data = _coders.at(id).decode(buffer, dataSize, blockSize);
    if (data != "")
    {
      handlePacket(data);
      // _header = DataHeader();
      //_coder = FecCoder();
    }
    lockk.unlock();
    lock.unlock();
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
std::string UdpReceiver::extractId(std::vector<uint8_t> &v, std::size_t &bytesTransferred)
{
  std::string str;
  for (size_t i = bytesTransferred - 8; i < bytesTransferred; i++)
  {
    str += static_cast<char>(v[i]); // Convert the uint8_t value to a character and append it to the string
  }
  v.erase(v.begin() + (bytesTransferred - 8), v.begin() + bytesTransferred);
  bytesTransferred -= 8;
  return std::move(str);
}
unsigned long UdpReceiver::extractIndex(std::vector<uint8_t> &v, std::size_t &bytesTransferred)
{
  uint8_t bytes[sizeof(long)];
  std::copy(v.begin() + (bytesTransferred - 8), v.begin() + bytesTransferred, bytes);
  long *value = reinterpret_cast<long *>(bytes);
  v.erase(v.begin() + (bytesTransferred - 8), v.begin() + bytesTransferred);
  bytesTransferred -= 8;
  return *value;
}

void UdpReceiver::processData(std::vector<std::vector<uint8_t>> v)
{
  for (int i = 0; i < v.size(); i++)
  {
    handleRawData(v.at(i), hSize - 1, packetCounter);
  }
}
