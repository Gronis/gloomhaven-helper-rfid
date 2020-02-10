#include <stdint.h>
#include <string>

#include "optional.hpp"

#include "protocol/deserializer.hpp"
#include "protocol/serializer.hpp"

using namespace ghh;
using namespace ghh::protocol;

Buffer::Buffer()
    : __data((uint8_t *)malloc(1024)),
        __dataLength(1024),
        __position(0),
        __free_on_delete(true)
{
}

Buffer::~Buffer()
{
    if(__free_on_delete){
        free(__data);
    }
}

Buffer::Buffer(
    uint8_t *data,
    std::size_t dataLength)
    : __data(data),
        __dataLength(dataLength),
        __position(0),
        __free_on_delete(false)
{
}

std::size_t Buffer::getPosition()
{
    return __position;
}

int Buffer::readInt(bool optimizePositive)
{
    tl::optional<int32_t> result;
    __position += ghh::protocol::readVarInt(
        __data + __position, __dataLength - __position, optimizePositive, result);
    return result? result.value() : 0;
}

void Buffer::writeInt(int value, bool optimizePositive)
{
    __position += ghh::protocol::writeVarInt(
        __data +  __position, __dataLength - __position, optimizePositive, value);
}

uint8_t Buffer::readByte()
{
    tl::optional<uint8_t> result;
    __position += ghh::protocol::readByte(
        __data + __position, __dataLength - __position, result);
    return result? result.value() : 0;
}

void Buffer::writeByte(uint8_t value)
{
    __position += ghh::protocol::writeByte(
        __data +  __position, __dataLength - __position, value);
}

int Buffer::readFullInt()
{
    tl::optional<int32_t> result;
    __position += ghh::protocol::readInt(
        __data + __position, __dataLength - __position, result);
    return result? result.value() : 0;
}

void Buffer::writeFullInt(int value)
{
    __position += ghh::protocol::writeInt(
        __data +  __position, __dataLength - __position, value);
}

tl::optional<std::string> Buffer::readString()
{
    tl::optional<std::string> result;
    __position += ghh::protocol::readString(
        __data + __position, __dataLength - __position, result);
    return result;
}

void Buffer::writeString(tl::optional<std::string> value)
{
    __position += ghh::protocol::writeString(
        __data +  __position, __dataLength - __position, value);
}

bool Buffer::readBoolean()
{
    return readByte();
}

void Buffer::writeBoolean(bool value)
{
    writeByte(value);
}

template <typename T>
void Buffer::writeEnum(T value)
{
    writeInt((int)value, true);
}

template <typename T>
T Buffer::readEnum(std::vector<T> values)
{
    return values[readInt(true)];
}

template <typename T>
void Buffer::writeEnumOrNull(tl::optional<T> value)
{
    if (!value)
        writeByte(0);
    else
        writeInt((int)value.value() + 1, true);
}

template <typename T>
tl::optional<T> Buffer::readEnumOrNull(std::vector<T> values)
{
    int value = readInt(true);
    if (value == 0)
        return tl::nullopt;
    return tl::optional<T>(values[value - 1]);
}

template <typename T>
int Buffer::writeArrayStart(std::vector<T> vec)
{
    int size = vec.size();
    writeInt(size, true);
    return size;
}

template <typename T>
void Buffer::writeEnumArray(std::vector<T> vec)
{
    int length = vec.size();
    writeInt(length, true);
    if (length == 0)
        return;
    for (int i = 0, n = vec.size(); i < n; i++)
        writeEnum(vec[i]);
}

template <typename T>
void Buffer::readEnumArray(std::vector<T> &vec, std::vector<T> values)
{
    int length = readInt(true);
    vec.reserve(length);
    for (int i = 0; i < length; i++)
        vec.push_back(readEnum(values));
}

void Buffer::writeIntArray(std::vector<int> vec, bool optimizePositive)
{
    int length = vec.size();
    writeInt(length, true);
    if (length == 0)
        return;
    for (int i = 0, n = vec.size(); i < n; i++)
        writeInt(vec[i], optimizePositive);
}
void Buffer::readIntArray(std::vector<int> &vec, bool optimizePositive)
{
    int length = readInt(true);
    vec.reserve(length);
    for (int i = 0; i < length; i++)
        vec.push_back(readInt(optimizePositive));
}
