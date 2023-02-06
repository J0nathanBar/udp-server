#include "MyDirectory.hpp"

MyDirectory::MyDirectory(boost::filesystem::path path, std::queue<std::vector<uint8_t>> &buf) : _path(path), _run(true), _buf(buf), _dirName(path.filename().string())
{
    t = std::thread(&MyDirectory::scanDir, this);
}

MyDirectory::~MyDirectory()
{
    t.join();
}
void MyDirectory::scanDir()
{
    while (_run)
    {
        if (!(_fileVec.empty()))
            _prevVec = std::move(_fileVec);

        if (!boost::filesystem::exists(_path))
        {
            std::cout << "Error: root path does not exist" << std::endl;
        }
        if (!boost::filesystem::is_directory(_path))
        {
            std::cout << "Error: root path is not a directory" << std::endl;
        }

        boost::filesystem::recursive_directory_iterator it(_path);
        boost::filesystem::recursive_directory_iterator end;

        while (it != end)
        {
            if (boost::filesystem::is_regular_file(it->path()))
            {

                ScannedFile(it->path());
            }
            ++it;
        }
    }
}

int MyDirectory::findFile(std::string path)
{

    for (int i = 0; i < _prevVec.size(); i++)
    {
        if (path.compare(_prevVec.at(i).getPath()->string()) == 0)
            return i;
    }
    return -1;
}

void MyDirectory::kill()
{
    _run = false;
}

std::string MyDirectory::getName()
{
    return _dirName;
}

void MyDirectory::ScannedFile(const boost::filesystem::path &k)
{

    int i = findFile(k.string());
    if (i != -1)
    {
        existingFile(k, i);
    }
    else
    {
        newFile(k);
    }
}
void MyDirectory::splitFile(std::string &data, int packetSize, std::string id)
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
        // _buf.emplace(packetData);
        _unEncoded.emplace_back(packetData);
    }
}
void MyDirectory::newFile(const boost::filesystem::path &k)
{

    boost::filesystem::path relativePath = boost::filesystem::relative(k.parent_path(), _path);
    // f.setRoot(relativePath.string());
    std::time_t time = boost::filesystem::last_write_time(k);
    ModifiedFile f(k, relativePath.string(), time);
    std::string parsedFile = _fParse.serialize(f);
    splitFile(parsedFile, 2000, f.getId());
    encode();
    _fileVec.push_back(f);
}
void MyDirectory::existingFile(const boost::filesystem::path &k, int i)
{
    if (_prevVec.at(i).getfTime() == boost::filesystem::last_write_time(k))
        _fileVec.push_back(_prevVec.at(i));
    else
        newFile(k);
}

void MyDirectory::encode()
{
    FecCoder coder;
    for (int i = 0; i < _unEncoded.size(); i++)
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "encoding chunk: " << i << std::endl;
        int packetSize = 1400;
        if (packetSize > _unEncoded.at(i).length())
        {
            packetSize = _unEncoded.at(i).length() / 3;
        }

        auto v = coder.encode(_unEncoded.at(i), packetSize); // change later to user transmitted
        mountOnBuffer(v);
    }
}
void MyDirectory::mountOnBuffer(std::shared_ptr<std::queue<std::vector<uint8_t>>> v)
{

    // FecCoder fc;

    while (!v->empty())
    {
        _buf.push(std::move(v->front()));
        v->pop();
    }
}
