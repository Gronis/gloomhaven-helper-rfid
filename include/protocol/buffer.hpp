#ifndef __PROTOCOL_MESSAGE_H__
#define __PROTOCOL_MESSAGE_H__

#include <stdint.h>
#include <string>

#include "optional.hpp"

#include "protocol/deserializer.hpp"
#include "protocol/serializer.hpp"

namespace ghr
{
namespace protocol
{

class Buffer
{
private:
    uint8_t *__data;
    std::size_t __dataLength;
    std::size_t __position;

    bool __free_on_delete;

public:
    Buffer()
        : __data((uint8_t *)malloc(1024)),
          __dataLength(1024),
          __position(0),
          __free_on_delete(true)
    {
    }

    ~Buffer()
    {
        if(__free_on_delete){
            free(__data);
        }
    }

    Buffer(
        uint8_t *data,
        std::size_t dataLength)
        : __data(data),
          __dataLength(dataLength),
          __position(0),
          __free_on_delete(false)
    {
    }

    std::size_t getPosition()
    {
        return __position;
    }

    int readInt(bool optimizePositive)
    {
        tl::optional<int32_t> result;
        __position += ghr::protocol::readVarInt(
            __data + __position, __dataLength - __position, optimizePositive, result);
        return result? result.value() : 0;
    }

    void writeInt(int value, bool optimizePositive)
    {
        __position += ghr::protocol::writeVarInt(
            __data +  __position, __dataLength - __position, optimizePositive, value);
    }

    uint8_t readByte()
    {
        tl::optional<uint8_t> result;
        __position += ghr::protocol::readByte(
            __data + __position, __dataLength - __position, result);
        return result? result.value() : 0;
    }

    void writeByte(uint8_t value)
    {
        __position += ghr::protocol::writeByte(
            __data +  __position, __dataLength - __position, value);
    }

    int readFullInt()
    {
        tl::optional<int32_t> result;
        __position += ghr::protocol::readInt(
            __data + __position, __dataLength - __position, result);
        return result? result.value() : 0;
    }

    void writeFullInt(int value)
    {
        __position += ghr::protocol::writeInt(
            __data +  __position, __dataLength - __position, value);
    }

    tl::optional<std::string> readString()
    {
        tl::optional<std::string> result;
        __position += ghr::protocol::readString(
            __data + __position, __dataLength - __position, result);
        return result;
    }

    void writeString(tl::optional<std::string> value)
    {
        __position += ghr::protocol::writeString(
            __data +  __position, __dataLength - __position, value);
    }

    bool readBoolean()
    {
        return readByte();
    }

    void writeBoolean(bool value)
    {
        writeByte(value);
    }

    template <typename T>
    void writeEnum(T value)
    {
        writeInt((int)value, true);
    }

    template <typename T>
    T readEnum(std::vector<T> values)
    {
        return values[readInt(true)];
    }

    template <typename T>
    void writeEnumOrNull(tl::optional<T> value)
    {
        if (!value)
            writeByte(0);
        else
            writeInt((int)value.value() + 1, true);
    }

    template <typename T>
    tl::optional<T> readEnumOrNull(std::vector<T> values)
    {
        int value = readInt(true);
        if (value == 0)
            return tl::nullopt;
        return tl::optional<T>(values[value - 1]);
    }

    template <typename T>
    int writeArrayStart(std::vector<T> vec)
    {
        int size = vec.size();
        writeInt(size, true);
        return size;
    }

    template <typename T>
    void writeEnumArray(std::vector<T> vec)
    {
        int length = vec.size();
        writeInt(length, true);
        if (length == 0)
            return;
        for (int i = 0, n = vec.size(); i < n; i++)
            writeEnum(vec[i]);
    }

    template <typename T>
    void readEnumArray(std::vector<T> &vec, std::vector<T> values)
    {
        int length = readInt(true);
        vec.reserve(length);
        for (int i = 0; i < length; i++)
            vec.push_back(readEnum(values));
    }

    void writeIntArray(std::vector<int> vec, bool optimizePositive)
    {
        int length = vec.size();
        writeInt(length, true);
        if (length == 0)
            return;
        for (int i = 0, n = vec.size(); i < n; i++)
            writeInt(vec[i], optimizePositive);
    }
    void readIntArray(std::vector<int> &vec, bool optimizePositive)
    {
        int length = readInt(true);
        vec.reserve(length);
        for (int i = 0; i < length; i++)
            vec.push_back(readInt(optimizePositive));
    }
};

} // namespace protocol
} // namespace ghr

#endif // __PROTOCOL_MESSAGE_H__