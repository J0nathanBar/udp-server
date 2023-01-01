#ifndef FILE_HPP
#define FILE_HPP
#pragma once
#include <boost/filesystem.hpp>
#include <iostream>
#include <string.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

class ModifiedFile
{
public:
    ModifiedFile(const boost::filesystem::path &path);
    ModifiedFile(std::string path);
    ModifiedFile(const boost::filesystem::path &,  boost::filesystem::path &);
    
    ModifiedFile();
    ~ModifiedFile();
    void setFile(boost::filesystem::path);
    void setPath(std::string folderPath);
    void setPath(boost::filesystem::path);
    void saveFile();
    boost::filesystem::path *getPath();
    std::string getFileName();
    std::string getData();
    int getSize();
    void setHandled(bool flag);
    bool getHandled();
    std::string getRootFolder();
    void setRoot(std::string);

private:
    void constructorDef();
    boost::filesystem::path _path;
    std::string _fileName;
    std::string _rootFolder;
    std::string _data;
    int _size;
    bool _beenHandled;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &_fileName;
        ar &_data;
        ar &_size;
        ar &_rootFolder;
    }
};
/*    std::cout << "filename and extension : " << src.filename() << std::endl; // file.ext
std::cout << "filename only          : " << src.stem() << std::endl;
std::cout << "extension only          : " << src.extension() << std::endl;  */
#endif