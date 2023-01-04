#ifndef FILEPACKET_HPP
#define FILEPACKET_HPP

#pragma once
#include <iostream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class FilePacket
{
public:
    FilePacket();
    FilePacket(std::string id, std::string data, unsigned long index, unsigned long last);
    ~FilePacket();
    std::string getData();
    std::string getId();
    unsigned long getIndex();
    unsigned long getLastPacket();

private:
    std::string _fileId;
    std::string _data;
    unsigned long _index;
    unsigned long _lastPacket;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &_fileId;
        ar &_data;
        ar &_index;
        ar &_lastPacket;
    }
};

#endif