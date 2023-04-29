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
    FileManager(std::queue<std::vector<uint8_t>> &buf, std::mutex &_bufferMutex);
    ~FileManager();
    void scanConf();
    bool handleFile(std::string &);
    bool handleDir(int chunkSize, int blockSize);
    bool getRun()
    {
        return _run;
    }
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
    std::queue<std::vector<uint8_t>> &_buf;
    std::vector<std::string> _unEncoded;
    std::thread t;
    std::mutex &_bufferMutex;
};

#endif