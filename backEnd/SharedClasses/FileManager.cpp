#include "FileManager.hpp"

FileManager::FileManager(std::queue<std::vector<uint8_t>> &buf, std::mutex &bufferMutex) : _path(""), _currentPath(""), _buf(buf),
                                                                                           _bufferMutex(bufferMutex)
{
}

FileManager::~FileManager()
{
    t.join();
    std::cout << "eeeee" << std::endl;
}

void FileManager::scanConf()
{

    while (_run)
    {

        try
        {
            int chunkSize, blockSize;
            bool flag = _jParse.convertJson(chunkSize, blockSize, _path);

            //  _path = _jParse.parse(_confPath);
            if (!flag)
                continue;
            boost::erase_all(_path, "\"");
            if (_path != _currentPath)
            {
                if (boost::filesystem::is_directory(_path))
                {
                    std::cout << "is dir" << std::endl;
                    std::cout << _path << std::endl;
                    _currentPath = _path;
                    handleDir(chunkSize, blockSize);
                }
                else if (boost::filesystem::is_regular_file(_path))
                {
                    std::cout << "is file" << std::endl;
                    _currentPath = _path;
                    handleFile(_currentPath);
                }
                else
                {
                    std::cout << "is not a file or folder" << std::endl;
                }
            }
        }
        catch (nlohmann::json::parse_error &ex)
        {
            std::cerr << ex.what() << std::endl;
        }
    }
}
bool FileManager::handleFile(std::string &path)
{
    try
    {

        ModifiedFile f(path);
        std::string id = f.getId();
    }
    catch (boost::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}
bool FileManager::handleDir(int chunkSize, int blockSize)
{
    try
    {
        MyDirectory dir(_currentPath, _buf, _bufferMutex, chunkSize, blockSize);
    }
    catch (boost::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}
void FileManager::kill()
{
    _run = false;
}
void FileManager::start()
{
    _run = true;
    t = std::thread(&FileManager::scanConf, this);
}

void FileManager::splitFile(std::string &data, int packetSize, std::string id)
{
    unsigned long index = 0;
    unsigned long lastPacket = data.length() / packetSize;
    if (lastPacket * packetSize != data.length())
        lastPacket++;
    for (unsigned long i = 0; i < data.length(); i += packetSize, index++)
    {
        // std::string splicedData = data.substr(i, packetSize);
        // FilePacket packet(id, splicedData, "", index);
        // packet.printInfo();
        // std::string packetData = _fParse.serialize(packet);
        // // _buf.emplace(packetData);
        // _unEncoded.emplace_back(packetData);
    }
}
void FileManager::encode()
{
    FecCoder coder;
    // for (int i = 0; i < _unEncoded.size(); i++)
    // {
    //     std::cout << "-----------------------------" << std::endl;
    //     std::cout << "encoding chunk: " << i << std::endl;
    //     int packetSize = 1400;
    //     if (packetSize > _unEncoded.at(i).length())
    //     {
    //         packetSize = _unEncoded.at(i).length() / 3;
    //     }

    //     auto v = coder.encode(_unEncoded.at(i), packetSize); // change later to user transmitted
    //     mountOnBuffer(v);
    // }
}
void FileManager::mountOnBuffer(std::shared_ptr<std::queue<std::vector<uint8_t>>> v)
{

    // FecCoder fc;

    while (!v->empty())
    {
        _buf.push(std::move(v->front()));
        v->pop();
    }
}
