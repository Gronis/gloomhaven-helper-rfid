#ifndef __PROTOCOL_MESSAGE_H__
#define __PROTOCOL_MESSAGE_H__

#include <stdint.h>
#include <string>
#include <vector>

#include "optional.hpp"
#include "protocol/deserializer.hpp"
#include "protocol/serializer.hpp"

namespace ghh
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

    inline void setPosition(std::size_t pos)
    {
        __position = pos;
    }
    inline std::size_t getPosition()
    {
        return __position;
    }

    inline int readInt(bool optimizePositive)
    {
        tl::optional<int32_t> result;
        __position += ghh::protocol::readVarInt(
            __data + __position, __dataLength - __position, optimizePositive, result);
        return result? result.value() : 0;
    }

    inline void writeInt(int value, bool optimizePositive)
    {
        __position += ghh::protocol::writeVarInt(
            __data +  __position, __dataLength - __position, optimizePositive, value);
    }

    inline uint8_t readByte()
    {
        tl::optional<uint8_t> result;
        __position += ghh::protocol::readByte(
            __data + __position, __dataLength - __position, result);
        return result? result.value() : 0;
    }

    inline void writeByte(uint8_t value)
    {
        __position += ghh::protocol::writeByte(
            __data +  __position, __dataLength - __position, value);
    }

    inline int readFullInt()
    {
        tl::optional<int32_t> result;
        __position += ghh::protocol::readInt(
            __data + __position, __dataLength - __position, result);
        return result? result.value() : 0;
    }

    inline void writeFullInt(int value)
    {
        __position += ghh::protocol::writeInt(
            __data +  __position, __dataLength - __position, value);
    }

    inline tl::optional<std::string> readString()
    {
        tl::optional<std::string> result;
        __position += ghh::protocol::readString(
            __data + __position, __dataLength - __position, result);
        return result;
    }

    inline void writeString(tl::optional<std::string> value)
    {
        __position += ghh::protocol::writeString(
            __data +  __position, __dataLength - __position, value);
    }

    inline bool readBoolean()
    {
        return readByte();
    }

    inline void writeBoolean(bool value)
    {
        writeByte(value);
    }

    template <typename T>
    inline void writeEnum(T value)
    {
        writeInt((int)value, true);
    }

    template <typename T>
    inline T readEnum(std::vector<T> values)
    {
        return values[readInt(true)];
    }

    template <typename T>
    inline void writeEnumOrNull(tl::optional<T> value)
    {
        if (!value)
            writeByte(0);
        else
            writeInt((int)value.value() + 1, true);
    }

    template <typename T>
    inline tl::optional<T> readEnumOrNull(std::vector<T> values)
    {
        int value = readInt(true);
        if (value == 0)
            return tl::nullopt;
        return tl::optional<T>(values[value - 1]);
    }

    template <typename T>
    inline int writeArrayStart(std::vector<T> vec)
    {
        int size = vec.size();
        writeInt(size, true);
        return size;
    }

    template <typename T>
    inline void writeEnumArray(std::vector<T> vec)
    {
        int length = vec.size();
        writeInt(length, true);
        if (length == 0)
            return;
        for (int i = 0, n = vec.size(); i < n; i++)
            writeEnum(vec[i]);
    }

    template <typename T>
    inline void readEnumArray(std::vector<T> &vec, std::vector<T> values)
    {
        int length = readInt(true);
        vec.reserve(length);
        for (int i = 0; i < length; i++)
            vec.push_back(readEnum(values));
    }

    inline void writeIntArray(std::vector<int> vec, bool optimizePositive)
    {
        int length = vec.size();
        writeInt(length, true);
        if (length == 0)
            return;
        for (int i = 0, n = vec.size(); i < n; i++)
            writeInt(vec[i], optimizePositive);
    }
    inline void readIntArray(std::vector<int> &vec, bool optimizePositive)
    {
        int length = readInt(true);
        vec.reserve(length);
        for (int i = 0; i < length; i++)
            vec.push_back(readInt(optimizePositive));
    }

};

} // namespace protocol
} // namespace ghh

#endif // __PROTOCOL_MESSAGE_H__