#include "UdpReceiver.hpp"

UdpReceiver::UdpReceiver(int port, boost::asio::io_service &context) : _port(port),
                                                                       _socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
                                                                       _run(true), _confPath("/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/RecvConf.json"), hcounter(0)

{
  std::cout << "starting server" << std::endl;
  _t = std::thread(&UdpReceiver::scanConf, this);
  _buffer.resize(100);
  packetCounter = 0;
  startReceive();
}
UdpReceiver::~UdpReceiver()
{
  _t.detach();
  for (int i = 0; i < _threads.size(); i++)
  {
    /* code */
    _threads.at(i).join();
  }
}
void UdpReceiver::startReceive()
{
  try
  {
    // std::cout << "recv: " << packetCounter << std::endl;
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
    std::vector<uint8_t> v = std::move(_buffer);

    if (!_header.isEmpty())
      _buffer.resize(_header.getBlockSize() + 100);
    else
      _buffer.resize(100);
    // std::cout << "---------------------------------------------------" << std::endl;
    // std::cout << "bytes transferred: " << bytesTransferred << std::endl;
    if (!ec && bytesTransferred > 0)
    {

      int id = v.at(--bytesTransferred);
      v.erase(v.begin() + bytesTransferred);
      if (id == 69 /*&& _header.isEmpty()*/)
      {
        handleHeader(bytesTransferred, v);
      }
      else if (id == 0 && !_header.isEmpty())
      {
        std::string str = extractId(v, bytesTransferred);
        unsigned long index = extractIndex(v, bytesTransferred);
        // std::cout << "index: " << index << std::endl;
        // std::cout << "header index: " << _header.getIndex() << std::endl;
        // std::cout << "id " << str << "header id: " << _header.getId() << std::endl;
        if (str == _header.getId() && index == _header.getIndex())
        {
          handleRawData(v);
        }
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
  FilePacket packet = fileSet.at(0);
  std::cout << packet.getFileData() << std::endl;
  ModifiedFile mf;
  _fParse.deSerialize(packet.getFileData(), mf);

  std::string p = _currentPath + mf.getRootFolder();
  if (!boost::filesystem::is_directory(p))
  {
    boost::filesystem::create_directories(p);
  }
  mf.setPath(p);
  std::ofstream File(mf.getPath().string(), std::ios::binary);
  if (!File.is_open())
  {
    throw std::runtime_error("Failed to open the file");
  }

  for (unsigned long i = 0; i < fileSet.size(); i++)
  {
    packet = fileSet.find(i)->second;
    // std::cout << "string: " << packet.getData() << std::endl;
    // std::cout << "string size " << packet.getData().size() << std::endl;
    File.write(packet.getData().data(), packet.getData().size());
  }

  File.close();
  std::cout << "File Saved" << std::endl;
}
void UdpReceiver::handlePacket(std::string data)
{
  FilePacket fp;
  _fParse.deSerialize(data, fp);
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
    if (fp.getLastPacket() == i->second.size())
    {
      stichFile(i->second);
      _packets.erase(id);
    }
   
    
  }
}

void UdpReceiver::handleHeader(int bytesTransferred, std::vector<uint8_t> &buffer)
{
  try
  {
    std::string hdata(_coder.decodeHeader(buffer, bytesTransferred));
    if (hdata != "")
    {
      _fParse.deSerialize(hdata, _header);
      _header.print();
      if (buffer.capacity() < _header.getBlockSize() + 100)
      {
        _buffer.resize(_header.getBlockSize() + 100);
      }
    }
  }
  catch (std::exception &e)
  {
    std::cout << "handle Header Excpetion: " << e.what() << std::endl;
  }
}
void UdpReceiver::handleRawData(std::vector<uint8_t> buffer)
{
  try
  {
    std::cout << "handling data " << std::endl;
    std::string data;
    data = _coder.decode(buffer, _header.getDataSize(), _header.getBlockSize());
    if (data != "")
    {
      _threads.push_back(std::thread(&UdpReceiver::handlePacket, this, data));
      _header = DataHeader();
      data = "";
      _coder = FecCoder();
      buffer.clear();
      buffer.resize(100);
    }
  }
  catch (std::exception &e)
  {
    std::cout << "handle raw Excpetion: " << e.what() << std::endl;
  }
}
void UdpReceiver::stichFile(std::vector<FilePacket> &vec)
{
  FilePacket packet = vec.at(0);
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