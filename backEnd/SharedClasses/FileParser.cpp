#include "FileParser.hpp"

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}
void FileParser::deSerialize(std::string data, ModifiedFile &f)
{

    std::istringstream stream(data);

    try
    {
        boost::archive::text_iarchive ar(stream);
        ar >> f;
    }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}
std::string FileParser::serialize(ModifiedFile &data)
{
    std::ostringstream stream;
    try
    {
        boost::archive::text_oarchive ar(stream);
        //  std::cout<<stream.str()<<std::endl;
        ar << data;
    }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return stream.str();
}

void FileParser::deSerialize(std::string data, FilePacket &fp)
{
    std::istringstream stream(data);
    try
    {
        boost::archive::text_iarchive ar(stream);
        ar >> fp;
    }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}
std::string FileParser::serialize(FilePacket &data)
{
    std::ostringstream stream;
    try
    {
        boost::archive::text_oarchive ar(stream);
        //  std::cout<<stream.str()<<std::endl;
        ar << data;
    }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return stream.str();
}