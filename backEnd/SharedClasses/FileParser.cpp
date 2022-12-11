#include "FileParser.hpp"

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}
void FileParser::deSerialize(std::string data,ModifiedFile * f)
{
    
    std::istringstream stream(data);
    
    try
    {
        boost::archive::text_iarchive ar(stream);
        ar >> *f;
    }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << e.what() << std::endl;
        throw e;
    }

   
}
std::string FileParser::serialize(ModifiedFile* data)
{
    std::ostringstream stream;
    try
    {
        boost::archive::text_oarchive ar(stream);
        ar << *data;
    }
    catch (boost::archive::archive_exception &e)
    {
        std::cout << e.what() << std::endl;
        throw e;
    }

    return stream.str();
}
