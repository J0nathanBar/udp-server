#include "FileParser.hpp"

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}

bool FileParser::deSerialize(const std::string &data, FilePacket &fp)
{
    std::istringstream stream(data);
    try
    {
        boost::archive::text_iarchive ar(stream);
        ar >> fp;
    }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << "file packet parse error: " << e.what() << std::endl;
        return false;
    }
    return true;
}
std::string FileParser::serialize(const FilePacket &data)
{
    std::ostringstream stream;
    try
    {
        boost::archive::text_oarchive ar(stream);
        ar << data;
    }
    catch (boost::archive::archive_exception &e)
    {

        std::cout << "parse error: " << e.what() << std::endl;
    }

    return stream.str();
}
bool FileParser::deSerialize(const std::string &data, DataHeader &h)
{

    std::istringstream stream(data);

    try
    {
        boost::archive::text_iarchive ar(stream);
        ar >> h;
        }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << "header parse error: " << e.what() << std::endl;
        return false;
    }
    h.fill();
    return true;
    /// h.print();
}
std::string FileParser::serialize(const DataHeader &data)
{
    std::ostringstream stream;
    try
    {
        boost::archive::text_oarchive ar(stream);
        ar << data;
    }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << "parse error: " << e.what() << std::endl;
    }

    return stream.str();
}