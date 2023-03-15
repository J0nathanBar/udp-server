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
#include <random>
#include <time.h>
#include <boost/container/map.hpp>
#include "FilePacket.hpp"
class ModifiedFile
{
public:
  ModifiedFile(const boost::filesystem::path &path);
  ModifiedFile(std::string path);
  ModifiedFile(const boost::filesystem::path &, boost::filesystem::path &);
  ModifiedFile(const boost::filesystem::path &path, const std::string, std::time_t);

  ModifiedFile(FilePacket);
  ModifiedFile();
  ~ModifiedFile();
  void setFile(boost::filesystem::path);
  void setPath(std::string folderPath);
  void setPath(boost::filesystem::path);
  void setfTime(std::time_t);
  std::time_t getfTime();
  boost::filesystem::path getPath();
  std::string getFileName();
  std::string getData();
  int getSize();
  void setHandled(bool flag);
  bool getHandled();
  std::string getRootFolder();
  void setRoot(std::string);
  std::string getId();
  void saveFile();
  bool saveFile(FilePacket &packet);
  void appendPacket(FilePacket);

private:
  void constructorDef();
  void generateId();
  boost::filesystem::path _path;
  std::string _fileName;
  std::string _rootFolder;
  std::string _id;
  unsigned long _size;
  bool _beenHandled;
  std::time_t _fTime;
  unsigned long _currentIndex, _lastPacket;
  boost::container::map<unsigned long, FilePacket> _packets;
};

#endif