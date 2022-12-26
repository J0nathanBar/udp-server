#include "MyDirectory.hpp"

MyDirectory::MyDirectory(boost::filesystem::path path, std::queue<std::string> &buf) : _path(path), _run(true), _buf(buf)
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
        //  _toSendVec.clear();
        _dirIt = boost::filesystem::directory_iterator{_path};
        while (_dirIt != boost::filesystem::directory_iterator{})
        {
            boost::filesystem::path k(*_dirIt++);
            int i = findFile(k.filename().string());
            if (i != -1)
            {
                _fileVec.push_back(_prevVec.at(i));
            }
            else
            {
                ModifiedFile f(&k);
                std::string parsedFile = _fParse.serialize(&f);
                _buf.push(parsedFile);
                _fileVec.push_back(f);
            }
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

void MyDirectory::kill()
{
    _run = false;
}
