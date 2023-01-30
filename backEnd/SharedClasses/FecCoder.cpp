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

FecCoder::~FecCoder()
{
}

std::shared_ptr<std::queue<std::vector<uint8_t>>> FecCoder::encode(const std::string &data, const int kPacketSize)
{
    _data = data;
    std::vector<uint8_t> msg(data.begin(), data.end());
    return encode(msg, kPacketSize);
}
std::shared_ptr<std::queue<std::vector<uint8_t>>> FecCoder::encode(const std::vector<uint8_t> &msg, const int kPacketSize)
{
    int i = 0;
    uint32_t writeLen = 0;
    _blockId = 0;
    int needed = 0;
    int kMessageBytes = msg.size();
    std::cout << "kMessageBytes: " << kMessageBytes << std::endl;

    _encoder = wirehair_encoder_create(nullptr, &msg[0], kMessageBytes, kPacketSize);
    if (!_encoder)
    {
        std::cout << "!!! Failed to create encoder" << std::endl;
        return nullptr;
    }
    auto v = std::make_shared<std::queue<std::vector<uint8_t>>>();
    makeHeader(v, kPacketSize, kMessageBytes);
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
        std::cout << "block size: " << block.size() << std::endl;
        v->emplace(block);
        i += writeLen;
        needed++;
    }
    wirehair_free(_encoder);
    return v;
}
bool FecCoder::decode(std::vector<uint8_t> &block, int kMessageBytes)

{
    if (!_decoder)
    {
        _decoder = wirehair_decoder_create(nullptr, kMessageBytes, block.size());
        _blockId = 1;
    }
    if (!_decoder)
    {
        // Free memory for encoder

        std::cout << "!!! Failed to create decoder" << std::endl;
        return false;
    }

    std::cout << "decode " << std::endl;
    WirehairResult decodeResult = wirehair_decode(
        _decoder,      // Decoder object
        _blockId,      // ID of block that was encoded
        &block[0],     // Input block
        block.size()); // Block length

    // If decoder returns success:
    if (decodeResult == Wirehair_Success)
    {
        // Decoder has enough data to recover now
        recover();
    }
    else if (decodeResult == Wirehair_NeedMore)
    {
        _blockId++;
        std::cout << "waiting for more data: " << std::endl;
    }
    else
    {
        std::cout << "wirehair_decoder failed: " << decodeResult << std::endl;
        return false;
    }
    return true;
}
bool FecCoder::recover()
{
    std::cout << "recover " << std::endl;
    std::vector<uint8_t> decoded(_data.length()); // change later

    WirehairResult decodeResult = wirehair_recover(_decoder, decoded.data(), decoded.size());
    if (decodeResult != Wirehair_Success)
    {
        std::cout << "wirehair_recover failed: " << decodeResult << std::endl;
        return false;
    }
    std::string a = std::string(decoded.begin(), decoded.end());
    std::cout << a << std::endl;
    _data = a;
    wirehair_free(_decoder);

    return true;
}
std::string FecCoder::getRecovered()
{
    return _data;
}
void FecCoder::makeHeader(std::shared_ptr<std::queue<std::vector<uint8_t>>> v, int kPacketSize, int kMessageBytes)
{
    std::cout << "header encoding starts" << std::endl;
    DataHeader h(kPacketSize, kMessageBytes);
    FileParser fp;
    int blockId = 0;
  
    std::string data = fp.serialize(h);
    std::vector<uint8_t> msg(data.begin(), data.end());
      int packetSize = msg.size() / 2 + 1;
      std::cout<<"header packet size: " << packetSize<<std::endl;

    WirehairCodec hencoder = wirehair_encoder_create(nullptr, &msg[0], msg.size(), packetSize);
    if (!hencoder)
    {
        std::cout << "!!! Failed to create header encoder" << std::endl;
        // return nullptr;
    }
    int i = 0;
    uint32_t writeLen = 0;
    while (i < msg.size())
    {
        std::vector<uint8_t> block(packetSize, 0);
        blockId++;
        WirehairResult encodeResult = wirehair_encode(
            hencoder,   // Encoder object
            blockId,   // ID of block to generate
            &block[0],  // Output buffer
            packetSize, // Output buffer size
            &writeLen); // Returned block length

        if (encodeResult != Wirehair_Success)
        {
            std::cout << "wirehair_encode header failed: " << encodeResult << std::endl;
        }
        std::cout << "header block size: " << block.size() << std::endl;
        v->emplace(block);
        i += writeLen;
    }
    wirehair_free(hencoder);
    std::cout << "header encoding over" << std::endl;
}