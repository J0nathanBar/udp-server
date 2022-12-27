#include "ModifiedFile.hpp"

ModifiedFile::ModifiedFile(std::string path) : _path(boost::filesystem::path(path)),_beenHandled(false)
{
    constructorDef();
}
ModifiedFile::ModifiedFile(boost::filesystem::path *path) : _path(*path)
{
    constructorDef();
}
ModifiedFile::ModifiedFile()
{
}
ModifiedFile::~ModifiedFile()
{
}

void ModifiedFile::constructorDef()
{
    boost::filesystem::ifstream f(_path);
    std::stringstream stream;
    stream << f.rdbuf();
    _data = stream.str();
    _fileName = _path.filename().string();
    _size = boost::filesystem::file_size(_path);
}

void ModifiedFile::setFile(boost::filesystem::path path)
{
    _path = boost::filesystem::path(path);
    constructorDef();
}
void ModifiedFile::setHandled(bool flag){
    _beenHandled = flag;
}
bool ModifiedFile::getHandled(){
    return _beenHandled;
}

boost::filesystem::path *ModifiedFile::getPath()
{
    return &_path;
}
std::string ModifiedFile::getData()
{
    return _data;
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
void ModifiedFile::setPath(boost::filesystem::path path){
    _path = path;
}