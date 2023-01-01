#include "MyDirectory.hpp"

MyDirectory::MyDirectory(boost::filesystem::path path, std::queue<std::string> &buf) : _path(path), _run(true), _buf(buf), _dirName(path.filename().string())
{
    t = std::thread(&MyDirectory::scanDir, this);
}
MyDirectory::MyDirectory(boost::filesystem::path path, std::queue<std::string> &buf, std::string root) : _path(path), _run(true), _buf(buf), _dirName(root + '/' + path.filename().string()), _dirRoot(root)
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
        if (!(_dirVec.empty()))
            _prevDirVec = std::move(_dirVec);
        // _dirIt = boost::filesystem::directory_iterator{_path};
        // while (_dirIt != boost::filesystem::directory_iterator{})
        // {
        //     boost::filesystem::path k(*_dirIt++);
        //      std::cout << k.filename().string()<< std::endl;
        //     if (boost::filesystem::is_regular_file(k))
        //     {

        //         ScannedFile(k);
        //     }
        //     else if (boost::filesystem::is_directory(k))
        //     {
        //         ScannedDir(k);
        //     }
        // }

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
            if (boost::filesystem::is_directory(it->path()))
            {
                //   ScannedDir(it->path());
            }
            else
            {

                ScannedFile(it->path());
            }
            ++it;
        }
    }
}

int MyDirectory::findFile(std::string name)
{

    for (int i = 0; i < _prevVec.size(); i++)
    {
        if (name.compare(_prevVec.at(i).getFileName()) == 0)
            return i;
    }
    return -1;
}
int MyDirectory::findDir(std::string name)
{
    for (int i = 0; i < _prevDirVec.size(); i++)
    {
        if (name.compare(_prevDirVec.at(i)) == 0)
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

    int i = findFile(k.filename().string());
    if (i != -1)
    {
        _fileVec.push_back(_prevVec.at(i));
    }
    else
    {
        boost::filesystem::path rel = boost::filesystem::relative(k, _path);
        ModifiedFile f(k, rel );
        std::cout << f.getData();
        std::string parsedFile = _fParse.serialize(&f);
        std::cout << parsedFile << std::endl;
        boost::filesystem::path relativePath = boost::filesystem::relative(k, _path);
        std::cout << relativePath << std::endl;
        _buf.push(parsedFile);
        _fileVec.push_back(f);
    }
}
