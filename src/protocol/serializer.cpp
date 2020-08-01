#include "protocol/serializer.hpp"

#include <string>

#include "utils.hpp"

using namespace ghh;
using namespace ghh::protocol;

std::size_t ghh::protocol::writeByte(uint8_t *buffer,
                                     const std::size_t bufferSize,
                                     const uint8_t data) {
  std::size_t count = 1;
  if (bufferSize >= count) {
    auto actualCount = ghh::copyBytes(&data, buffer, count, bufferSize);
    if (actualCount == count) {
      return actualCount;
    }
  }
  return 0;
}
std::size_t ghh::protocol::writeShort(uint8_t *buffer,
                                      const std::size_t bufferSize,
                                      const int16_t data) {
  std::size_t count = 2;
  if (bufferSize >= count) {
    const uint8_t *dataAsArray = reinterpret_cast<const uint8_t *>(&data);
    std::size_t actualCount =
        ghh::copyBytes(dataAsArray, buffer, count, bufferSize);
    if (actualCount == count) {
      reverse(count, buffer);
      return actualCount;
    }
  }
  return 0;
}
std::size_t ghh::protocol::writeInt(uint8_t *buffer,
                                    const std::size_t bufferSize,
                                    const int32_t data) {
  std::size_t count = 4;
  if (bufferSize >= count) {
    const uint8_t *dataAsArray = reinterpret_cast<const uint8_t *>(&data);
    std::size_t actualCount =
        ghh::copyBytes(dataAsArray, buffer, count, bufferSize);
    if (actualCount == count) {
      reverse(count, buffer);
      return actualCount;
    }
  }
  return 0;
}
std::size_t ghh::protocol::writeVarInt(uint8_t *buffer,
                                       const std::size_t bufferSize,
                                       bool optimizePositive,
                                       const int32_t data) {
  uint32_t uData = data;
  if (!optimizePositive) {
    if (data >= 0) {
      uData = ((uint32_t)data) << 1;
    } else {
      uData = (((uint32_t)-data) << 1) - 1;
    }
  }
  uint8_t carry = true;
  std::size_t i = 0;
  for (; i < 5 && carry; i++) {
    uint8_t b = (uint8_t)uData;
    uData = uData >> 7;
    carry = uData > 0;
    b |= carry ? 0x80 : 0;
    auto ok = writeByte(buffer + i, bufferSize - i, b) == 1;
    if (!ok) {
      return 0;
    }
  }
  return i;
}

std::size_t ghh::protocol::writeUTFString(uint8_t *buffer,
                                          const std::size_t bufferSize,
                                          const std::string &data) {
  std::size_t count = 0;
  count += writeShort(buffer + count, bufferSize - count, data.length());
  const uint8_t *dataArr = reinterpret_cast<const uint8_t *>(data.c_str());
  count += ghh::copyBytes(dataArr, buffer + count, data.length(),
                          bufferSize - count);
  if (count != data.length() + 2) {
    return 0;  // Buffer is full
  }
  return count;
}

std::size_t ghh::protocol::writeUTFStringLength(uint8_t *buffer,
                                                const std::size_t bufferSize,
                                                const int32_t &data) {
  uint32_t uData = data;
  uint8_t carry = true;
  std::size_t i = 0;
  for (; i < 5 && carry; i++) {
    uint8_t b = (uint8_t)uData;
    uData = uData >> (7 - (i == 0));
    carry = uData > 0;
    b |= i == 0 || carry ? 0x80 : 0;
    b |= i == 0 && carry ? 0x40 : 0;
    auto ok = writeByte(buffer + i, bufferSize - i, b) == 1;
    if (!ok) {
      return 0;
    }
  }
  return i;
}
std::size_t ghh::protocol::writeUTFStringValue(uint8_t *buffer,
                                               const std::size_t bufferSize,
                                               const std::string &data) {
  const uint8_t *dataArr = reinterpret_cast<const uint8_t *>(data.c_str());
  return copyBytes(dataArr, buffer, data.length(), bufferSize);
}

std::size_t ghh::protocol::writeAscii(uint8_t *buffer,
                                      const std::size_t bufferSize,
                                      const std::string &data) {
  if (data.length() < 2) {
    return 0;
  }
  std::size_t count = 0;
  const uint8_t *dataArr = reinterpret_cast<const uint8_t *>(data.c_str());
  // TODO: This breaks on non ascii characters, and we don't detect it
  count += ghh::copyBytes(dataArr, buffer + count, data.length(),
                          bufferSize - count);
  if (count != data.length()) {
    return 0;
  }
  buffer[count - 1] |= 0x80;
  return count;
}

std::size_t ghh::protocol::writeString(uint8_t *buffer,
                                       const std::size_t bufferSize,
                                       const tl::optional<std::string> data) {
  // Null string is value 128 in this representation
  std::size_t count = 0;
  if (!data.has_value()) {
    return writeByte(buffer + count, bufferSize - count, 128);
  }
  const std::string &value = data.value();
  bool useAscii = false;  // value.length() > 1 && value.length() < 64;
  if (useAscii) {
    count += writeAscii(buffer + count, bufferSize - count, value);
  }
  // If ascii fails, use utf8
  if (count == 0) {
    // Count nr of characters in string
    std::size_t character_count = 0;
    uint8_t c = 0;
    for (std::size_t i = 0; i < value.length(); character_count++) {
      c = value[i];
      i += utf8CharacterByteCount(c);
    }
    if (bufferSize <= character_count) {
      return 0;
    }
    std::size_t countLength = writeUTFStringLength(
        buffer + count, bufferSize - count, character_count + 1);
    count += countLength;
    std::size_t countValue =
        writeUTFStringValue(buffer + count, bufferSize - count, value);
    count += countValue;
    if (countLength == 0 || countValue == 0) {
      return 0;
    }
  }
  return count;
}