#include "MyDirectory.hpp"

MyDirectory::MyDirectory(boost::filesystem::path path, std::queue<std::string> &buf) : _path(path), _run(true), _buf(buf), _dirName(path.filename().string())
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
        _fileVec.push_back(_prevVec.at(i));
    }
    else
    {
        boost::filesystem::path rel = boost::filesystem::relative(k, _path.parent_path().parent_path());
        ModifiedFile f(k);
        boost::filesystem::path relativePath = boost::filesystem::relative(k.parent_path(), _path);
        std::cout << relativePath << std::endl;
        f.setRoot(relativePath.string());
        std::string parsedFile = _fParse.serialize(f);

        std::cout << "relatives: " << f.getRootFolder() << std::endl;
        _buf.push(parsedFile);
        _fileVec.push_back(f);
    }
}
void MyDirectory::splitFile(std::string data){
    

}
