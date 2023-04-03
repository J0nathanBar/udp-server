#include "MyDirectory.hpp"

MyDirectory::MyDirectory(boost::filesystem::path path, std::queue<std::vector<uint8_t>> &buf, std::mutex &bufferMutex, int chunkSize, int blockSize) : _path(path), _run(true), _buf(buf), _dirName(path.filename().string()), _bufferMutex(bufferMutex),
                                                                                                                                                       _sem(_MaxThreads)
{
    t = std::thread(&MyDirectory::scanDir, this, chunkSize, blockSize);
    cleaner = std::thread(&MyDirectory::cleanThreads, this);
}

MyDirectory::~MyDirectory()
{
    t.join();
    cleaner.join();
    for (auto &thread : _threads)
    {
        thread.join();
    }
}
void MyDirectory::scanDir(int chunkSize, int blockSize)
{
    int i = 0;
    while (_run && i++ == 0) // remove later!!!!
    {
        //  std::lock_guard<std::mutex> guard(_vecMutex);

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

                ScannedFile(it->path(), chunkSize, blockSize);
            }
            ++it;
        }
        std::cout << "finished Scanning" << std::endl;

        running = false;
    }
}

int MyDirectory::findFile(std::string path)
{

    for (int i = 0; i < _prevVec.size(); i++)
    {
        if (path.compare(_prevVec.at(i).getPath().string()) == 0)
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

void MyDirectory::ScannedFile(const boost::filesystem::path &k, int chunkSize, int blockSize)
{

    int i = findFile(k.string());
    if (i != -1)
    {
        existingFile(k, i, chunkSize, blockSize);
    }
    else
    {
        boost::filesystem::path a(k);
        // newFile(k);
        _sem.acquire();
        std::unique_lock<std::mutex> lock(_threadMutex);
        _threads.push_back(std::thread(&MyDirectory::newFile, this, std::move(a), chunkSize, blockSize));
        lock.unlock();

        // std::thread b(&MyDirectory::newFile, this, k);
        // b.detach();
    }
}
std::vector<std::string> MyDirectory::splitFile(ModifiedFile &f, int chunkSize, std::string &id, const boost::filesystem::path &path)
{
    unsigned long index = 0;

    std::vector<FilePacket> packets;
    std::vector<std::string> unEncoded;
    std::ifstream File(path.string(), std::ios::binary);
    if (!File.is_open())
    {
        std::cout << path.string() << std::endl;
        throw std::runtime_error("Failed to open the file");
    }

    while (File.good())
    {

        std::string chunk(chunkSize, '\0');

        File.read(&chunk[0], chunkSize);
        unsigned long size = f.getSize();
        std::string name = f.getFileName();
        std::string root = f.getRootFolder();
        packets.emplace_back(FilePacket(id, chunk, index, size, name, root));
        index++;
    }
    File.close();
    for (FilePacket p : packets)
    {
        p.setLastPacket(packets.size());
        std::string packetData = _fParse.serialize(p);
        unEncoded.emplace_back(packetData);
    }
    std::cout << "last packet " << packets.size() << std::endl;
    return std::move(unEncoded);
}
void MyDirectory::newFile(const boost::filesystem::path k, int chunkSize, int blockSize)
{
    boost::filesystem::path relativePath = boost::filesystem::relative(k.parent_path(), _path);
    std::time_t time = boost::filesystem::last_write_time(k);
    ModifiedFile f(k, relativePath.string(), time);

    std::string id = f.getId();

    int size = f.getSize();
    if (size == 0)
    {
        return;
    }
    if (size < chunkSize)
        chunkSize = size + 1;
    std::string root = f.getRootFolder();
    std::vector<std::string> unEncoded = splitFile(f, chunkSize, id, k);
    id = f.getId();
    encode(id, std::move(unEncoded), blockSize);
    // std::lock_guard<std::mutex> guard(_vecMutex);
    // _fileVec.push_back(f);
    _sem.release();
}
void MyDirectory::existingFile(const boost::filesystem::path &k, int i, int chunkSize, int blockSize)
{
    if (_prevVec.at(i).getfTime() == boost::filesystem::last_write_time(k))
    {
        // std::lock_guard<std::mutex> guard(_vecMutex);

        // _fileVec.push_back(_prevVec.at(i));
    }
    else
        newFile(k, chunkSize, blockSize);
}

void MyDirectory::encode(std::string &id, std::vector<std::string> unEncoded, int blockSize)
{
    std::shared_ptr<FecCoder> coder = std::make_shared<FecCoder>(generateHeaderId());
    // FecCoder coder;
    for (unsigned long i = 0; i < unEncoded.size(); i++)
    {
        if (blockSize > unEncoded.at(i).length())
        {
            blockSize = unEncoded.at(i).length() / 10;
        }
        auto v = coder->encode(unEncoded.at(i), blockSize, id, i);
        mountOnBuffer(v);
    }
}
void MyDirectory::mountOnBuffer(std::shared_ptr<std::queue<std::vector<uint8_t>>> v)
{
    FecCoder c;
    std::unique_lock<std::mutex> lock(_bufferMutex);
    uint8_t id = _headerId;
    std::vector<uint8_t> a(1, 0);

    if (_buf.size() + v->size() < _MaxThreads)
    {
        while (!v->empty())
        {
            _buf.push(std::move(v->front()));
            v->pop();
        }
        _buf.push(a);
        lock.unlock();
    }
    else
    {
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        mountOnBuffer(v);
    }
}
std::string MyDirectory::generateHeaderId()
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(8) << _hCounter;
    _hCounter++;
    return ss.str();
}
void MyDirectory::cleanThreads()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::unique_lock<std::mutex> lock(_threadMutex);
        for (size_t i = 0; i < _threads.size(); i++)
        {
            if (_threads.at(i).joinable())
            {
                _threads.at(i).join();
                _threads.erase(_threads.begin() + i);
            }
        }
        lock.unlock();
    }
}
