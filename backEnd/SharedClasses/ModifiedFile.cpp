#include "ModifiedFile.hpp"

ModifiedFile::ModifiedFile(const boost::filesystem::path &path, const std::string root, int chunkSize, int blockSize) : _path(path), _rootFolder(root), _blockSize(blockSize), _chunkSize(chunkSize), _mounted(false), _packetsSent(0)
{
    _firstDetected = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    //  _firstDetected = static_cast<unsigned long>(now.time_since_epoch().count());
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

void ModifiedFile::setStartEncode()
{
    _startEncode = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
}

void ModifiedFile::setEndEncode()
{
    _endEnocde = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
}

void ModifiedFile::setMountTime()
{
    _mountTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    _mounted = true;
}

void ModifiedFile::addPacketsSent(int p)
{
    _packetsSent += p;
}

bool ModifiedFile::isMounted()
{
    return _mounted;
}

void ModifiedFile::printData()
{
    std::cout << "Data: " << std::endl;

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(_endEnocde - _startEncode);

    int dur = static_cast<int>(elapsed_time.count());
    std::cout << "encode time: " << dur << std::endl;

    long start_time_long = static_cast<long>(_startEncode.time_since_epoch().count()) / 1000000;
    long end_time_long = static_cast<long>(_endEnocde.time_since_epoch().count()) / 1000000;

    // Calculate duration as long
    long duration_long = end_time_long - start_time_long;
    std::cout << "long res: " << duration_long << std::endl;
}

void ModifiedFile::insertToTxTable()
{
    //_firstDetected, _startEncode, _endEnocde, _mountTime;
    std::string first = std::to_string(static_cast<long>(_firstDetected.time_since_epoch().count()) / 1000000);
    std::string startEncode = std::to_string(static_cast<long>(_startEncode.time_since_epoch().count()) / 1000000);
    std::string endEncode = std::to_string(static_cast<long>(_endEnocde.time_since_epoch().count()) / 1000000);
    std::string mount = std::to_string(static_cast<long>(_mountTime.time_since_epoch().count()) / 1000000);

    MYSQL *connection = mysql_init(NULL);
    mysql_real_connect(connection, "localhost", "broman", "yeet", "my_database", 0, NULL, 0);
    if (!connection)
    {
        std::cout << "failed connection" << std::endl;

        return;
    }

    std::string query = "INSERT INTO TX_TABLE (file_id, file_size, block_size,chunk_size,packets_sent,first_detected_time,start_encode_time,end_encode_time,mount_time) VALUES ('" + _id + "', '" + std::to_string(_size) + "', '" + std::to_string(_blockSize) + "', '" + std::to_string(_chunkSize) + "', '" + std::to_string(_packetsSent) + "', '" + first + "', '" + startEncode + "', '" + endEncode + "', '" + mount + "')";
    if (mysql_query(connection, query.c_str()))
    {
        std::cout << "error!!! coudlnt insert!" << std::endl;
        mysql_close(connection);
        return;
    }

    mysql_close(connection);
}
