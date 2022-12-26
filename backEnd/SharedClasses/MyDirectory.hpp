#ifndef MYDIRECTORY_HPP
#define MYDIRECTORY_HPP

#pragma once
#include "ModifiedFile.hpp"
#include "FileParser.hpp"
#include "boost/container/vector.hpp"
#include <queue>
#include <thread>

class MyDirectory
{
public:
    MyDirectory(boost::filesystem::path,std::queue<std::string> & );
    ~MyDirectory();
    void scanDir();
    int findFile(std::string name);

    void kill();

private:
    boost::container::vector<ModifiedFile> _fileVec;
    boost::container::vector<ModifiedFile> _prevVec;
    std::queue<std::string> & _buf;
    boost::filesystem::path _path;
    boost::filesystem::directory_iterator _dirIt;
    bool _run;
    std::thread t;
    FileParser _fParse;
};

#endif