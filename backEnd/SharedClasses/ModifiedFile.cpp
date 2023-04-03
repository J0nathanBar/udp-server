#include "ModifiedFile.hpp"

ModifiedFile::ModifiedFile(const boost::filesystem::path &path) : _path((path)), _beenHandled(false)
{
    _rootFolder = "";
    constructorDef();
}
ModifiedFile::ModifiedFile(const boost::filesystem::path &path, const std::string root, std::time_t time) : _path(path), _rootFolder(root), _fTime(time)
{
    constructorDef();
}

ModifiedFile::ModifiedFile(std::string path) : _path((path)), _beenHandled(false)
{
}
ModifiedFile::ModifiedFile(const boost::filesystem::path &path, boost::filesystem::path &root) : _path(path), _rootFolder(root.string())
{
    constructorDef();
}
ModifiedFile::ModifiedFile(FilePacket fp) : _currentIndex(0), _beenHandled(false), _path(fp.getPath()), _size(fp.getSize()), _id(fp.getId()), _lastPacket(fp.getLastPacket())
{
    appendPacket(std::move(fp));
}
ModifiedFile::ModifiedFile()
{
}
ModifiedFile::~ModifiedFile()
{
}

void ModifiedFile::constructorDef()
{
    generateId();
    boost::filesystem::ifstream f(_path);
    std::stringstream stream;
    stream << f.rdbuf();
    // _data = stream.str();
    _fileName = _path.filename().string();
    _size = boost::filesystem::file_size(_path);
}

void ModifiedFile::setFile(boost::filesystem::path path)
{
    _path = boost::filesystem::path(path);
    constructorDef();
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
std::string ModifiedFile::getData()
{
    // return _data;
}

std::string ModifiedFile::getFileName()
{
    return _fileName;
}
int ModifiedFile::getSize()
{
    return _size;
}
void ModifiedFile::setPath(std::string folderPath)
{
    _path = folderPath;
    _path.append(_fileName);
}
void ModifiedFile::setPath(boost::filesystem::path path)
{
    _path = path;
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
    static const char alphabet[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";

    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<int> distribution(0, sizeof(alphabet) - 2);

    std::string result;
    result.reserve(8);

    for (int i = 0; i < 8; ++i)
    {
        result += alphabet[distribution(generator)];
    }
    _id = result;
}
std::string ModifiedFile::getId()
{
    return _id;
}
void ModifiedFile::saveFile()
{
    auto it = _packets.rbegin();
    int maxKey = it->first;
 //   std::cout << "awaiting: " << _currentIndex << " currently in the system: " << maxKey << std::endl;
    while (!_beenHandled && !_packets.empty() && _packets.begin()->first == _currentIndex)
    {
        bool flag = saveFile(_packets.at(_currentIndex));
        _packets.erase(_currentIndex - 1);
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
std::time_t ModifiedFile::getfTime()
{
    return _fTime;
}
void ModifiedFile::setfTime(std::time_t time)
{
    _fTime = time;
}