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
        std::string parsedFile = _fParse.serialize(&f);
        _buf.push(parsedFile);
        // post(parsedFile);
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
        /* code */
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