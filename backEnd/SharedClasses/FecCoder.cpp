#include "FecCoder.hpp"

FecCoder::FecCoder()
{
    const WirehairResult initResult = wirehair_init();
    if (initResult != Wirehair_Success)
    {
        std::cout << "!!! Wirehair initialization failed: " << initResult << std::endl;
    }
    _blockId = 0;
    rec = false;
}

FecCoder::FecCoder(std::string hID) : _hID(std::move(hID)), _hBytes(std::vector<uint8_t>(_hID.begin(), _hID.end()))
{
    const WirehairResult initResult = wirehair_init();
    if (initResult != Wirehair_Success)
    {
        std::cout << "!!! Wirehair initialization failed: " << initResult << std::endl;
    }
    _blockId = 0;
    rec = false;
}

FecCoder::~FecCoder()
{
}

std::shared_ptr<std::queue<std::vector<uint8_t>>> FecCoder::encode(const std::string &data, const int kPacketSize, const std::string id, const unsigned long index)
{
    _data = data;
    std::vector<uint8_t> msg(data.begin(), data.end());
    return encode(msg, kPacketSize, id, index);
}
std::shared_ptr<std::queue<std::vector<uint8_t>>> FecCoder::encode(const std::vector<uint8_t> &msg, const int kPacketSize, const std::string id, const unsigned long index)
{
    int i = 0;
    uint32_t writeLen = 0;
    _blockId = 0;
    int needed = 0;
    int kMessageBytes = msg.size();
    _encoder = wirehair_encoder_create(nullptr, &msg[0], kMessageBytes, kPacketSize);
    if (!_encoder)
    {
        std::cout << "!!! Failed to create encoder" << std::endl;
        return nullptr;
    }
    auto v = std::make_shared<std::queue<std::vector<uint8_t>>>();
    makeHeader(v, kPacketSize, kMessageBytes, id, index);
    while (i < msg.size())
    {
        std::vector<uint8_t> block(kPacketSize, 0);
        _blockId++;
        WirehairResult encodeResult = wirehair_encode(
            _encoder,    // Encoder object
            _blockId,    // ID of block to generate
            &block[0],   // Output buffer
            kPacketSize, // Output buffer size
            &writeLen);  // Returned block length

        if (encodeResult != Wirehair_Success)
        {
            std::cout << "wirehair_encode failed: " << encodeResult << std::endl;
            return nullptr;
        }
        block.insert(block.end(), _hBytes.begin(), _hBytes.end());
        block.emplace_back(0);
        v->emplace(block);
        i += writeLen;
        needed++;
    }
    wirehair_free(_encoder);
    _blockId = 0;
    return v;
}
std::string FecCoder::decode(std::vector<uint8_t> &block, int kMessageBytes, int kPacketSize)
{
    if (_blockId == 0)
    {
        _decoder = wirehair_decoder_create(nullptr, kMessageBytes, kPacketSize);
        _blockId++;
    }
    if (!_decoder)
    {

        _blockId = 0;
        std::cout << "!!! Failed to create decoder" << std::endl;
        std::cout << "msg bytes: " << kMessageBytes << " packet Size" << kPacketSize << std::endl;
        return ".";
    }
    WirehairResult decodeResult = wirehair_decode(
        _decoder,     // Decoder object
        _blockId,     // ID of block that was encoded
        &block[0],    // Input block
        kPacketSize); // Block length

    // If decoder returns success:
    if (decodeResult == Wirehair_Success)
    {
        // Decoder has enough data to recover now
        return recover(kMessageBytes);
    }
    else if (decodeResult == Wirehair_NeedMore)
    {
        _blockId++;
    }
    else
    {
        std::cout << "wirehair_decoder failed: " << decodeResult << std::endl;
        return ".";
    }
    return "";
}
std::string FecCoder::recover(int kMessageBytes)
{
    std::vector<uint8_t> decoded(kMessageBytes); // change later
    WirehairResult decodeResult = wirehair_recover(_decoder, decoded.data(), kMessageBytes);
    if (decodeResult != Wirehair_Success)
    {
        std::cout << "wirehair_recover failed: " << decodeResult << std::endl;
        return ".";
    }
    std::string a = std::string(decoded.begin(), decoded.end());
    //   std::cout << a << std::endl;
    _data = a;
    _blockId = 0;
    wirehair_free(_decoder);
    return a;
}

void FecCoder::makeHeader(std::shared_ptr<std::queue<std::vector<uint8_t>>> v, int kPacketSize, int kMessageBytes, const std::string id, const unsigned long index)
{
    //   std::cout << "header encoding starts" << std::endl;
    DataHeader h(kPacketSize, kMessageBytes, id, index, _hID);
    FileParser fp;
    int blockId = 0;
    std::string data = fp.serialize(h);
    std::vector<uint8_t> msg(data.begin(), data.end());
    int headerSize = data.size();
    int headerPacketSize = headerSize / 12;
    WirehairCodec hencoder = wirehair_encoder_create(nullptr, &msg[0], headerSize, headerPacketSize);
    if (!hencoder)
    {
        std::cout << "!!! Failed to create header encoder" << std::endl;
        // return nullptr;
    }
    int i = 0;
    uint32_t writeLen = 0;
    while (i < headerSize)
    {
        std::vector<uint8_t> block(headerPacketSize, 0);
        blockId++;
        WirehairResult encodeResult = wirehair_encode(
            hencoder,         // Encoder object
            blockId,          // ID of block to generate
            &block[0],        // Output buffer
            headerPacketSize, // Output buffer size
            &writeLen);       // Returned block length

        if (encodeResult != Wirehair_Success)
        {
            std::cout << "wirehair_encode header failed: " << encodeResult << std::endl;
        }
        block.emplace_back(headerSize);
        block.emplace_back(69);
        v->emplace(block);

        i += writeLen;
    }
    wirehair_free(hencoder);
    //   std::cout << "header encoding over" << std::endl;
}
std::string FecCoder::decodeHeader(std::vector<uint8_t> &block, int kPacketSize)
{
    kPacketSize--;
    int headerSize = block.at(kPacketSize);
    block.pop_back();

    decode(block, headerSize, kPacketSize);
    if (_data != "")
    {
        return std::move(_data);
    }
    return "";
}
void FecCoder::LongtoVec(std::vector<uint8_t> &v, unsigned long value)
{
}
