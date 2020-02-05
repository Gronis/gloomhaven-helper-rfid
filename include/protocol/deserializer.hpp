#ifndef __PROTOCOL_DESERIALIZER_H__
#define __PROTOCOL_DESERIALIZER_H__

#include <vector>
#include <iostream>

#include "optional.hpp"
#include "utils.hpp"
#include "protocol/header.hpp"

namespace ghr
{
namespace protocol
{

std::size_t copyBytes(
    const uint8_t *src, uint8_t *dest, std::size_t count, const std::size_t bufferSize)
{
    if(count >= bufferSize)
    {
        count = bufferSize;
    }
    std::memcpy(reinterpret_cast<void*>(dest),reinterpret_cast<const void*>(src),count);
    return count;
    return 0;

    // Old implementation can be found in readBytes in InputStream.hpp
}

std::size_t readByte(
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

std::size_t readShort(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int16_t> &out)
{
    int16_t data;
    std::size_t count = 2;
    if (bufferSize >= count)
    {
        uint8_t *dataAsArray = reinterpret_cast<uint8_t *>(&data);
        std::size_t actualCount = copyBytes(buffer, dataAsArray, count, bufferSize);
        reverse(count, dataAsArray);
        if (actualCount == count){
            out = data;
            return actualCount;
        }
    }
    out = tl::nullopt;
    return 0;
}

std::size_t readInt(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int32_t> &out)
{
    int32_t data;
    std::size_t count = 4;
    if (bufferSize >= count)
    {
        uint8_t *dataAsArray = reinterpret_cast<uint8_t *>(&data);
        std::size_t actualCount = copyBytes(buffer, dataAsArray, count, bufferSize);
        reverse(count, dataAsArray);
        if (actualCount == count){
            out = data;
            return actualCount;
        }
    }
    out = tl::nullopt;
    return 0;
}

std::size_t readVarInt(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int32_t> &out)
{
    int32_t b = 0;
    int32_t result = 0;
    uint8_t varIntBuffer[5];
    std::size_t count = 5;
    auto peekedBytes = copyBytes(buffer, varIntBuffer, count, bufferSize);
    for (auto i = 0; i <= 4; i++)
    {
        if (peekedBytes <= i){
            out = tl::nullopt;
            return 0;
        }

        uint8_t b = varIntBuffer[i];
        result |= (b & 0x7F) << (i * 7);
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

std::size_t readUTFString(
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


// Attempts to read a message
// Returns the number of bytes read.
// The header is put in the out argument
std::size_t readHeader(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<Header> &out)
{
    tl::optional<std::string> message;
    tl::optional<int32_t> length;
    std::size_t bytesRead = 0;
    bytesRead += readUTFString(buffer + bytesRead, bufferSize - bytesRead, message);
    bytesRead += readVarInt(buffer + bytesRead, bufferSize - bytesRead, length);
    if(!message.has_value() || !length.has_value()){
        out = tl::nullopt;
        return 0;
    }
    std::string msg = message.value();
    int32_t len = length.value();
    std::size_t index = msg.find(" ");
    out = Header();
    if (index != -1)
    {
        out->event = trim(msg.substr(0, index));
        out->payload = trim(msg.substr(index + 1));
    }
    else
    {
        out->event = trim(msg);
        out->payload = "";
    }
    out->length = len;
    return bytesRead;
}

} // namespace protocol
} // namespace ghr

#endif // __PROTOCOL_DESERIALIZER_H__