#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#pragma once
#include "JsonParser.hpp"
#include "FileParser.hpp"
#include "MyDirectory.hpp"
#include "FilePacket.hpp"
#include <thread>
#include <queue>
#include <vector>
#include "FecCoder.hpp"
class FileManager
{
public:
    FileManager(std::queue<std::vector<uint8_t>> &buf);
    ~FileManager();
    void scanConf();
    bool handleFile(std::string &);
    bool handleDir();
    void kill();
    void start();

private:
    void encode();
    void mountOnBuffer(std::shared_ptr<std::queue<std::vector<uint8_t>>> v);
    void splitFile(std::string &data, int packetSize, std::string id);

    JsonParser _jParse;
    FileParser _fParse;
    bool _run;
    std::string _path;
    std::string _currentPath;
    std::string _confPath;
    std::queue<std::vector<uint8_t>> &_buf;
    std::vector<std::string> _unEncoded;
    std::thread t;
};

#endif