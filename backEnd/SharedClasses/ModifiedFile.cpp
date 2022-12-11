#include "ModifiedFile.hpp"

ModifiedFile::ModifiedFile(std::string path) : _path(boost::filesystem::path(path))
{
    boost::filesystem::ifstream f(_path);
    std::stringstream stream;
    stream << f.rdbuf();
    _data = stream.str();
    _fileName = _path.filename().string();
    _size = boost::filesystem::file_size(path);
}
ModifiedFile::ModifiedFile(){
    
}
ModifiedFile::~ModifiedFile()
{
}

void ModifiedFile::setFile(std::string path)
{
    _path = boost::filesystem::path(path);
    boost::filesystem::ifstream f(_path);
    std::stringstream stream;
    stream << f.rdbuf();
    _data = stream.str();
    _fileName = _path.filename().string();
    _size = boost::filesystem::file_size(path);
}

boost::filesystem::path * ModifiedFile::getPath(){
    return &_path;
}
std::string ModifiedFile::getData(){
    return _data;
}
std::string ModifiedFile::getFileName(){
    return _fileName;
}
int ModifiedFile::getSize(){
    return _size;
}
void ModifiedFile::setPath(std::string folderPath){
    _path = folderPath;
    _path.append(_fileName);

}