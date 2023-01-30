#ifndef MYDIRECTORY_HPP
#define MYDIRECTORY_HPP

#pragma once
#include "ModifiedFile.hpp"
#include "FileParser.hpp"
#include "FileManager.hpp"
#include "boost/container/vector.hpp"
#include <queue>
#include <wirehair/wirehair.h>
#include <vector>
#include <thread>

class MyDirectory
{
public:
    MyDirectory(boost::filesystem::path, std::queue<std::vector<uint8_t>> &);
    ~MyDirectory();
    void scanDir();
    int findFile(std::string name);
    std::string getName();
    void kill();
    void splitFile(std::string data, int packetSize, std::string id);
    void ScannedFile(const boost::filesystem::path &);
    void newFile(const boost::filesystem::path &);
    void existingFile(const boost::filesystem::path &, int);
    void encode(std::vector<std::string> &chunks,int packetSize,std::string data);

private:
    boost::container::vector<ModifiedFile> _fileVec;
    boost::container::vector<ModifiedFile> _prevVec;
    std::queue<std::vector<uint8_t>> &_buf;
    boost::filesystem::path _path;
    boost::filesystem::directory_iterator _dirIt;
    std::string _dirName;
    std::string _dirRoot;
    bool _run;
    std::thread t;
    FileParser _fParse;
    //std::vector<std::string> _chunks;

};

#endif