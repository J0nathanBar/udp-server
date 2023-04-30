#ifndef FILEPACKET_HPP
#define FILEPACKET_HPP

#pragma once
#include <iostream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/filesystem.hpp>

class FilePacket
{
public:
    FilePacket(unsigned int packetsDecoded, unsigned long headerCreated, unsigned long lastDetected, unsigned long startDecode);
    FilePacket(std::string id, std::string data, unsigned long index, unsigned long size, std::string fileName, std::string rootFolder);
    ~FilePacket();
    std::string getData();
    std::string getFileData();
    std::string getId();
    std::string getFileName();
    std::string getRootFolder();
    unsigned long getSize();
    unsigned long getIndex();
    unsigned long getLastPacket();
    void printInfo();
    void setLastPacket(unsigned long);
    void setPath(std::string);
    boost::filesystem::path getPath();
    unsigned long getRecovered();
    unsigned int getPacketsDecoded();
    unsigned long getHeaderCreated();
    unsigned long getLastDetected();
    unsigned long getStartDecode();

private:
    std::string _fileId, _fileName, _rootFolder;
    std::string _data, _fileData;
    boost::filesystem::path _path;
    unsigned long _index;
    unsigned long _lastPacket;
    unsigned long _size;
    std::chrono::high_resolution_clock::time_point _recovered;
    unsigned long _headerCreated, _lastDetected, _startDecode;
    unsigned int _packetsDecoded;
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &_fileId;
        //   ar &_fileData;
        ar &_data;
        ar &_index;
        ar &_lastPacket;
        ar &_size;
        ar &_fileName;
        ar &_rootFolder;
    }
};

#endif