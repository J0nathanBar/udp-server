#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#pragma once
#include "JsonParser.hpp"
#include "FileParser.hpp"
#include "MyDirectory.hpp"
#include "FilePacket.hpp"
#include <thread>
#include <queue>
class FileManager
{
public:
    FileManager(std::queue<std::string> &buf);
    ~FileManager();
    void scanConf();
    bool handleFile(std::string);
    bool handleDir();
    void kill();
    void start();
    void SplitFile(std::string data,int packetSize,std::string id);

private:
    JsonParser _jParse;
    FileParser _fParse;
    bool _run;
    std::string _path;
    std::string _currentPath;
    std::string _confPath;
    std::queue<std::string> &_buf;
    std::thread t;
};

#endif