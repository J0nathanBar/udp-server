#include "ModifiedFile.hpp"

ModifiedFile::ModifiedFile(const boost::filesystem::path &path, const std::string root, int chunkSize, int blockSize) : _path(path), _rootFolder(root), _blockSize(blockSize), _chunkSize(chunkSize)
{
    auto now = std::chrono::system_clock::now();
    _firstDetected = static_cast<unsigned long>(now.time_since_epoch().count());
    _fileName = _path.filename().string();
    _size = boost::filesystem::file_size(_path);
    generateId();
}

ModifiedFile::ModifiedFile(FilePacket fp) : _currentIndex(0), _beenHandled(false), _path(fp.getPath()), _size(fp.getSize()), _id(fp.getId()), _lastPacket(fp.getLastPacket())
{
    appendPacket(std::move(fp));
}

ModifiedFile::~ModifiedFile()
{
}

void ModifiedFile::setHandled(bool flag)
{
    _beenHandled = flag;
}
bool ModifiedFile::getHandled()
{
    return _beenHandled;
}

boost::filesystem::path ModifiedFile::getPath()
{
    return _path;
}

std::string ModifiedFile::getFileName()
{
    return _fileName;
}
int ModifiedFile::getSize()
{
    return _size;
}

std::string ModifiedFile::getRootFolder()
{
    return _rootFolder;
}
void ModifiedFile::setRoot(std::string root)
{
    _rootFolder = root;
}

void ModifiedFile::generateId()
{
    boost::uuids::random_generator generator;
    boost::uuids::uuid uuid = generator();
    _id = boost::uuids::to_string(uuid);
}
std::string ModifiedFile::getId()
{
    return _id;
}
void ModifiedFile::saveFile()
{
    auto it = _packets.rbegin();
    int maxKey = it->first;
    std::cout << "awaiting: " << _currentIndex << " currently in the system: " << maxKey << std::endl;
    while (!_beenHandled && !_packets.empty() && _packets.begin()->first == _currentIndex)
    {
        bool flag = saveFile(_packets.at(_currentIndex));
        _packets.erase(_currentIndex - 1);
        //   std::cout << _packets.size() << std::endl;
    }
}
bool ModifiedFile::saveFile(FilePacket &packet)
{
    try
    {
        std::ofstream File(_path, std::ios::app);
        if (_currentIndex == 0)
            std::ofstream File(_path, std::ios::binary);

        if (!File.is_open())
        {
            std::cout << _path << std::endl;
            throw std::runtime_error("Failed to open the file");
            return false;
        }
        File.write(packet.getData().data(), packet.getData().size());
        File.close();
        _currentIndex++;
        if (_currentIndex == _lastPacket)
        {
            _beenHandled = true;
            std::cout << "file saved" << std::endl;
            // TODO delelte headers for the greater good!!!
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}
void ModifiedFile::appendPacket(FilePacket packet)
{
    _packets.emplace(packet.getIndex(), std::move(packet));
    saveFile();
}

void ModifiedFile::setStartEncode(auto t)
{
    _startEncode = static_cast<unsigned long>(t.time_since_epoch().count());
}

void ModifiedFile::setEndEncode(auto t)
{
    _endEnocde = static_cast<unsigned long>(t.time_since_epoch().count());
}

void ModifiedFile::setMountTime(auto t)
{
    _mountTime = static_cast<unsigned long>(t.time_since_epoch().count());
}

void ModifiedFile::setPacketsSent(int p)
{
    _packetsSent = p;
}
