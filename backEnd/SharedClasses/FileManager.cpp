#include "FileManager.hpp"

FileManager::FileManager(std::queue<std::string> &buf) : _path(""), _currentPath(""), _buf(buf),
                                                         _confPath("/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/TransConf.json"), _run(false)
{
}

FileManager::~FileManager()
{
    t.join();
}

void FileManager::scanConf()
{
    while (_run)
    {
        try
        {
            _path = _jParse.parse(_confPath);
            boost::erase_all(_path, "\"");
            if (_path != _currentPath)
            {
                if (boost::filesystem::is_directory(_path))
                {
                    std::cout << "is dir" << std::endl;
                    _currentPath = _path;
                    handleDir();
                }
                else if (boost::filesystem::is_regular_file(_path))
                {
                    std::cout << "is file" << std::endl;
                    _currentPath = _path;
                    handleFile(_currentPath);
                }
            }
        }
        catch (nlohmann::json::parse_error &ex)
        {
            std::cerr << ex.what() << std::endl;
        }
    }
}
bool FileManager::handleFile(std::string path)
{
    try
    {
        ModifiedFile f(path);
        std::cout << "file in folder: " << f.getFileName() << std::endl;
        std::string id = f.getId();
        std::string parsedFile = _fParse.serialize(f);
        splitFile(parsedFile, 2000, id);
    }
    catch (boost::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}
bool FileManager::handleDir()
{
    try
    {
        MyDirectory dir(_currentPath, _buf);
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

void FileManager::splitFile(std::string data, int packetSize, std::string id)
{
    unsigned long index = 0;
    unsigned long lastPacket = data.length() / packetSize;
    if (lastPacket * packetSize != data.length())
        lastPacket++;
    for (unsigned long i = 0; i < data.length(); i += packetSize, index++)
    {
        std::string splicedData = data.substr(i, packetSize);
        FilePacket packet(id, splicedData, index, lastPacket);
        packet.printInfo();
        std::string packetData = _fParse.serialize(packet);
        _buf.emplace(packetData);
    }
}