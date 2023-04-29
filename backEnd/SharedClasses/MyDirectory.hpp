#ifndef MYDIRECTORY_HPP
#define MYDIRECTORY_HPP

#pragma once
#include "ModifiedFile.hpp"
#include "FileParser.hpp"
#include "FileManager.hpp"
#include "boost/container/vector.hpp"
#include <queue>
#include <wirehair/wirehair.h>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <semaphore>
#include <boost/thread.hpp>

class MyDirectory
{
public:
    MyDirectory(boost::filesystem::path, std::queue<std::vector<uint8_t>> &, std::mutex &bufferMutex, int chunkSize, int blockSize);
    ~MyDirectory();
    void scanDir(int chunkSize, int blockSize);
    std::string getName();
    void kill();
    std::vector<std::string> splitFile(std::shared_ptr<ModifiedFile> f, int packetSize, std::string &id, const boost::filesystem::path &path);
    void ScannedFile(const boost::filesystem::path &, int chunkSize, int blockSize);
    void newFile(const boost::filesystem::path, int chunkSize, int blockSize);
    void encode(std::string &id, std::vector<std::string> unEncoded, int blockSize, std::shared_ptr<ModifiedFile> f);
    void mountOnBuffer(std::shared_ptr<std::queue<std::vector<uint8_t>>> v, std::shared_ptr<ModifiedFile> f);
    std::string generateHeaderId();
    void cleanThreads();
    bool running = true;

private:
    std::vector<std::string> _unEncoded;
    std::queue<std::vector<uint8_t>> &_buf;
    boost::filesystem::path _path;
    boost::filesystem::directory_iterator _dirIt;
    std::string _dirName;
    std::string _dirRoot;
    bool _run;
    std::thread t, cleaner;
    std::mutex _vecMutex, _threadMutex;
    std::mutex &_bufferMutex;
    std::vector<std::thread> _threads;
    FileParser _fParse;
    const int _MaxThreads = 400000;
    std::counting_semaphore<> _sem;
    int _pz, _kmsg;
    const int _headerId = 69;
    const int _dataId = 0;
    int _hCounter = 0;
    std::time_t _loopTime;
    bool _firstRun;
};

#endif