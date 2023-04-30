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
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <mysql/mysql.h>
class ModifiedFile
{
public:
  ModifiedFile(const boost::filesystem::path &path, const std::string, int chunkSize, int blockSize);

  ModifiedFile(FilePacket);

  ~ModifiedFile();

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

  void setStartEncode();
  void setEndEncode();
  void setMountTime();
  void addPacketsSent(int p);
  bool isMounted();
  void printData();
  void insertToTxTable();
  void insertToRxTable();

private:
  void generateId();
  boost::filesystem::path _path;
  std::string _fileName;
  std::string _rootFolder;
  std::string _id;
  unsigned long _size;
  bool _beenHandled;
  unsigned long _currentIndex, _lastPacket;
  unsigned int _chunkSize, _blockSize;
  boost::container::map<unsigned long, FilePacket> _packets;
  bool _mounted;
  std::chrono::high_resolution_clock::time_point _firstDetected, _startEncode, _endEnocde, _mountTime, _saveTIme;
  unsigned int _packetsSent, _packetsDecoded;
  unsigned long _firstheaderCreated, _lastDetected, _startDecode, _recovered;
};

#endif