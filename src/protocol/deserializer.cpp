#include <string>
#include <cstring>

#include "protocol/deserializer.hpp"
#include "utils.hpp"

using namespace ghh;
using namespace ghh::protocol;

std::size_t ghh::protocol::readByte(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<uint8_t> &out)
{
    uint8_t data;
    std::size_t count = 1;
    if (bufferSize >= count)
    {
        auto actualCount = copyBytes(buffer, &data, count, bufferSize);
        if (actualCount == count){
            out = data;
            return actualCount;
        }
    }
    out = tl::nullopt;
    return 0;
}

std::size_t ghh::protocol::readShort(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int16_t> &out)
{
    int16_t data;
    std::size_t count = 2;
    if (bufferSize >= count)
    {
        uint8_t *dataAsArray = reinterpret_cast<uint8_t *>(&data);
        std::size_t actualCount = copyBytes(buffer, dataAsArray, count, bufferSize);
        if (actualCount == count){
            reverse(count, dataAsArray);
            out = data;
            return actualCount;
        }
    }
    out = tl::nullopt;
    return 0;
}

std::size_t ghh::protocol::readInt(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int32_t> &out)
{
    int32_t data;
    std::size_t count = 4;
    if (bufferSize >= count)
    {
        uint8_t *dataAsArray = reinterpret_cast<uint8_t *>(&data);
        std::size_t actualCount = copyBytes(buffer, dataAsArray, count, bufferSize);
        if (actualCount == count){
            reverse(count, dataAsArray);
            out = data;
            return actualCount;
        }
    }
    out = tl::nullopt;
    return 0;
}

std::size_t ghh::protocol::readVarInt(
    const uint8_t *buffer, const std::size_t bufferSize,
    bool optimizePositive, tl::optional<int32_t> &out)
{
    int32_t b = 0;
    int32_t result = 0;
    uint8_t varIntBuffer[5];
    std::size_t count = 5;
    auto actualCount = copyBytes(buffer, varIntBuffer, count, bufferSize);
    for (auto i = 0; i < actualCount; i++)
    {
        uint8_t b = varIntBuffer[i];
        result |= (b & 0x7F) << (i * 7);
        if ((b & 0x80) == 0)
        {
            out = optimizePositive ? result : ((((unsigned int)result) >> 1) ^ -(result & 1));
            return i + 1;
        }
    }
    // All bytes not available, or (invalid 32-bit integer, should never happen, invalid input)
    out = tl::nullopt;
    return 0;
}

std::size_t ghh::protocol::readUTFString(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<std::string> &out)
{
    tl::optional<int16_t> stringLength;
    auto bytesRead = readShort(buffer, bufferSize, stringLength);
    if(!stringLength.has_value() || bufferSize - bytesRead < stringLength.value())
    {
        out = tl::nullopt;
        return 0;
    }
    auto strLen = stringLength.value();
    if(strLen == 0)
    {
        out = "";
        return bytesRead;
    }
    // Has read size of utf string. Read the actual string
    char data[strLen + 1];
    data[strLen] = 0;
    auto size = copyBytes(buffer + bytesRead, reinterpret_cast<uint8_t *>(data), strLen, bufferSize - bytesRead);
    bytesRead += size;
    if (size == strLen)
    {
        out = std::string(data);
        return bytesRead;
    }
    out = tl::nullopt;
    return 0;
}


std::size_t ghh::protocol::readUTFStringLength(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int32_t> &out)
{
    int32_t result = 0;
    uint8_t varIntBuffer[5];
    std::size_t count = 5;
    auto actualCount = copyBytes(buffer, varIntBuffer, count, bufferSize);
    uint8_t b = varIntBuffer[0];

    result |= (b & 0x3F);
    if ((b & 0x40) == 0)
    {
        out = result;
        return 1;
    }

    for (auto i = 1; i < actualCount; i++)
    {
        uint8_t b = varIntBuffer[i];
        result |= (b & 0x7F) << (i * 7 - 1);
        if ((b & 0x80) == 0)
        {
            out = result;
            return i + 1;
        }
    }
    // This should never happen, (invalid 32-bit integer)
    out = tl::nullopt;
    return 0;
}

std::size_t ghh::protocol::readUTFStringValue(
    const uint8_t *buffer, const std::size_t bufferSize,
    std::size_t stringLength, tl::optional<std::string> &out)
{
    char data[stringLength + 1];
    data[stringLength] = 0;
    auto size = copyBytes(buffer, reinterpret_cast<uint8_t *>(data), stringLength, bufferSize);
    if (size == stringLength)
    {
        out = std::string(data);
        return size;
    }
    out = tl::nullopt;
    return 0;
}

std::size_t ghh::protocol::readAscii(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<std::string> &out)
{
    std::size_t count = 0;
    tl::optional<uint8_t> b;
    do
    {
        count += readByte(buffer + count, bufferSize - count, b);
    } while (b.has_value() && (b.value() & 0x80) == 0);

    // Enouth data is not available
    if(!b.has_value()){
        out = tl::nullopt;
        return 0;
    }

    char data[count + 1];
    std::memcpy(reinterpret_cast<void*>(data), reinterpret_cast<const void*>(buffer), count);
    data[count - 1] &= 0x7F; // Mask end of ascii bit.
    data[count] = 0; // Mark end
    out = std::string(data);
    return count;
}

std::size_t ghh::protocol::readString(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<std::string> &out)
{
    tl::optional<uint8_t> b;
    std::size_t count = 0;
    count += readByte(buffer + count, bufferSize - count, b);
    if(!b.has_value())
    {
        out = tl::nullopt;
        return 0;
    }
    count --;
    // ASCII.
    if ((b.value() & 0x80) == 0)
    {
        count += readAscii(buffer + count, bufferSize - count, out);
        // std::cout << "ascii " << count << out.value() << "\n";
        return count;
    }
    // Null, empty, or UTF8.
    tl::optional<int32_t> size;
    count += readUTFStringLength(buffer + count, bufferSize - count, size);
    if(size){
        switch (size.value())
        {
            case 0:
                out = tl::nullopt;
                return 1;
            case 1:
                out = "";
                return 1;
        }
    }
    // Because size 1 means empty string, reduce size by one
    count += readUTFStringValue(buffer + count, bufferSize - count, size.value() - 1, out);
    return count;
}
