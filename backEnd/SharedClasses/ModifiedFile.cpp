#include "ModifiedFile.hpp"

ModifiedFile::ModifiedFile(const boost::filesystem::path &path, const std::string root, int chunkSize, int blockSize) : _path(path), _rootFolder(root), _blockSize(blockSize), _chunkSize(chunkSize), _mounted(false), _packetsSent(0)
{
    _firstDetected = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    //  _firstDetected = static_cast<unsigned long>(now.time_since_epoch().count());
    _fileName = _path.filename().string();
    _size = boost::filesystem::file_size(_path);
    if (_chunkSize > _size)
        _chunkSize = _size;
    generateId();
}

ModifiedFile::ModifiedFile(FilePacket fp) : _currentIndex(0), _beenHandled(false), _path(fp.getPath()), _size(fp.getSize()), _id(fp.getId()), _lastPacket(fp.getLastPacket()), _packetsDecoded(0), _firstheaderCreated(0), _startDecode(0), _recovered(0), _lastDetected(0)
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
    //  std::cout << "awaiting: " << _currentIndex << " currently in the system: " << maxKey << std::endl;
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
            _saveTIme = std::chrono::system_clock::now();
            insertToRxTable();
            std::cout << "file saved" << std::endl;
            _beenHandled = true;
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

    _packetsDecoded += packet.getPacketsDecoded();
    unsigned long hCreated = packet.getHeaderCreated();
    if (_firstheaderCreated == 0 || hCreated < _firstheaderCreated)
        _firstheaderCreated = hCreated;
    unsigned long startDec = packet.getStartDecode();
    if (_startDecode == 0 || startDec < _startDecode)
        _startDecode = startDec;
    unsigned long lastDetect = packet.getLastDetected();
    if (_lastDetected == 0 || lastDetect > _lastDetected)
        _lastDetected = lastDetect;
    unsigned long recTime = packet.getRecovered();
    if (_recovered == 0 || recTime > _recovered)
        _recovered = recTime;
    _packets.emplace(packet.getIndex(), std::move(packet));
    saveFile();
}

void ModifiedFile::setBlockSize(int blockSize)
{
    _blockSize = blockSize;
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
}

void ModifiedFile::insertToTxTable()
{
    //_firstDetected, _startEncode, _endEnocde, _mountTime;
    std::string first = std::to_string(static_cast<unsigned long>(_firstDetected.time_since_epoch().count()) / 1000000);
    std::string startEncode = std::to_string(static_cast<unsigned long>(_startEncode.time_since_epoch().count()) / 1000000);
    std::string endEncode = std::to_string(static_cast<unsigned long>(_endEnocde.time_since_epoch().count()) / 1000000);
    std::string mount = std::to_string(static_cast<unsigned long>(_mountTime.time_since_epoch().count()) / 1000000);

    MYSQL *connection = mysql_init(NULL);

    if (!connection)
    {
        std::cout << "failed connection" << std::endl;

        return;
    }
    if (!mysql_real_connect(connection, "localhost", "broman", "yeet", "my_database", 0, NULL, 0))
    {
        std::cout << "Error: can't connect to MySQL server." << std::endl;
        std::cout << "MySQL error message: " << mysql_error(connection) << std::endl;
        mysql_close(connection);
        return;
    }
    std::cout << "connected!" << std::endl;

    std::string query = "INSERT INTO TX_TABLE (file_id, file_size, block_size,chunk_size,packets_sent,first_detected_time,start_encode_time,end_encode_time,mount_time) VALUES ('" + _id + "', " + std::to_string(_size) + ", " + std::to_string(_blockSize) + ", " + std::to_string(_chunkSize) + ", " + std::to_string(_packetsSent) + ", " + first + ", " + startEncode + ", " + endEncode + ", " + mount + ")";
    if (mysql_query(connection, query.c_str()))
    {
        std::cout << "error!!! " << mysql_error(connection) << std::endl;
        std::cout << "query: \n\n\n\n\n"
                  << query << std::endl;
        mysql_close(connection);
        return;
    }
    std::cout << "TX injected: " << _id << std::endl;

    mysql_close(connection);
}

void ModifiedFile::insertToRxTable()
{
    std::string saved = std::to_string(static_cast<unsigned long>(_saveTIme.time_since_epoch().count()) / 1000000);

    std::string packetsDecoded = std::to_string(_packetsDecoded);
    std::string firstDected = std::to_string(_firstheaderCreated);
    std::string startDecode = std::to_string(_startDecode);
    std::string lastDetected = std::to_string(_lastDetected);
    std::string recovered = std::to_string(_recovered);

    MYSQL *connection = mysql_init(NULL);
    if (!connection)
    {
        std::cout << "failed connection" << std::endl;

        return;
    }
    if (!mysql_real_connect(connection, "localhost", "broman", "yeet", "my_database", 0, NULL, 0))
    {
        std::cout << "Error: can't connect to MySQL server." << std::endl;
        std::cout << "MySQL error message: " << mysql_error(connection) << std::endl;
        mysql_close(connection);
        return;
    }

    std::string query = "INSERT INTO RX_TABLE (file_id, packets_decoded,first_time_detected,last_time_detected,recovered_time, saved_time ) VALUES ('" + _id + "', " + packetsDecoded + ", " + firstDected + +", " + lastDetected + ", " + recovered + ", " + saved + ")";
    if (mysql_query(connection, query.c_str()))
    {
        std::cout << "error!!! " << mysql_error(connection) << std::endl;
        std::cout << query << std::endl;
        mysql_close(connection);
        return;
    }
    std::cout << "RX injected: " << _id << std::endl;

    mysql_close(connection);
}
