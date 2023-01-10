#ifndef MYDIRECTORY_HPP
#define MYDIRECTORY_HPP

#pragma once
#include "ModifiedFile.hpp"
#include "FileParser.hpp"
#include "FileManager.hpp"
#include "boost/container/vector.hpp"
#include <queue>
#include <thread>

class MyDirectory
{
public:
    MyDirectory(boost::filesystem::path, std::queue<std::string> &);
    MyDirectory();
    ~MyDirectory();
    void scanDir();
    int findFile(std::string name);
    int findDir(std::string name);
    std::string getName();
    void kill();

    void splitFile(std::string data, int packetSize, std::string id);
    void ScannedFile(const boost::filesystem::path &);
    void ScannedDir(const boost::filesystem::path &);

private:
    boost::container::vector<ModifiedFile> _fileVec;
    boost::container::vector<ModifiedFile> _prevVec;
    boost::container::vector<std::string> _dirVec;
    boost::container::vector<std::string> _prevDirVec;
    std::queue<std::string> &_buf;
    boost::filesystem::path _path;
    boost::filesystem::directory_iterator _dirIt;
    std::string _dirName;
    std::string _dirRoot;
    bool _run;
    std::thread t;
    FileParser _fParse;
};

#endif