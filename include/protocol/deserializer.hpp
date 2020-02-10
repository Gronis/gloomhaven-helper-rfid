#ifndef __PROTOCOL_DESERIALIZER_H__
#define __PROTOCOL_DESERIALIZER_H__

#include "optional.hpp"
#include "protocol/header.hpp"

namespace ghh
{
namespace protocol
{

std::size_t readByte(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<uint8_t> &out);

std::size_t readShort(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int16_t> &out);

std::size_t readInt(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int32_t> &out);

std::size_t readVarInt(
    const uint8_t *buffer, const std::size_t bufferSize,
    bool optimizePositive, tl::optional<int32_t> &out);

// There are 3 different ways of reading strings
//
// readUTFString, 2 bytes int the beginning that defines the length
// readString: 1 of 2 different encodings
//    ascii encoding, read ascii characters until one character with bit 8 set to 1
//    utf encoding, if bit 8 is 1 on first character, read length like readVarInt, but shifted 1 bit
//          to the right. Then read that number of bytes.
std::size_t readUTFString(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<std::string> &out);

std::size_t readUTFStringLength(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<int32_t> &out);

std::size_t readUTFStringValue(
    const uint8_t *buffer, const std::size_t bufferSize,
    std::size_t stringLength, tl::optional<std::string> &out);

std::size_t readAscii(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<std::string> &out);

std::size_t readString(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<std::string> &out);

// Attempts to read a message
// Returns the number of bytes read.
// The header is put in the out argument
std::size_t readHeader(
    const uint8_t *buffer, const std::size_t bufferSize, tl::optional<Header> &out);

} // namespace protocol
} // namespace ghh

#endif // __PROTOCOL_DESERIALIZER_H__