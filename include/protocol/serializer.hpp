#ifndef __PROTOCOL_SERIALIZER_H__
#define __PROTOCOL_SERIALIZER_H__

#include "optional.hpp"

namespace ghh
{
namespace protocol
{

std::size_t writeByte(uint8_t *buffer, const std::size_t bufferSize, const uint8_t data);
std::size_t writeShort(uint8_t *buffer, const std::size_t bufferSize, const int16_t data);
std::size_t writeInt(uint8_t *buffer, const std::size_t bufferSize, const int32_t data);
std::size_t writeVarInt(
    uint8_t *buffer, const std::size_t bufferSize, bool optimizePositive, const int32_t data);

std::size_t writeAscii(uint8_t *buffer, const std::size_t bufferSize, const std::string &data);

std::size_t writeString(
    uint8_t *buffer, const std::size_t bufferSize, const tl::optional<std::string> data);

std::size_t writeUTFString(uint8_t *buffer, const std::size_t bufferSize, const std::string& data);

} // namespace protocol
} // namespace ghh

#endif // __PROTOCOL_SERIALIZER_H__