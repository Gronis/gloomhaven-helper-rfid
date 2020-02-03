#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdint.h>
#include <string>

#include "optional.hpp"
#include "encoding.hpp"
#include "decoding.hpp"

namespace ghr
{

class Message
{
private:
    uint8_t *__data;
    std::size_t __dataLength;
    std::size_t __position;

    bool __free_on_delete;

public:
    Message()
        : __data((uint8_t *)malloc(1024)),
          __dataLength(1024),
          __position(0),
          __free_on_delete(true)
    {
    }

    ~Message()
    {
        if(__free_on_delete){
            free(__data);
        }
    }

    Message(
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
        return ghr::readInt(__data, __position, optimizePositive);
    }

    void writeInt(int value, bool optimizePositive)
    {
        ghr::writeInt(__data, __position, value, optimizePositive);
    }

    uint8_t readByte()
    {
        return ghr::readByte(__data, __position);
    }

    void writeByte(uint8_t value)
    {
        ghr::writeByte(__data, __position, value);
    }

    bool readBoolean()
    {
        return readByte();
    }

    void writeBoolean(bool value)
    {
        writeByte(value);
    }

    int readFullInt()
    {
        int value;
        reinterpret_cast<uint8_t *>(&value)[3] = readByte();
        reinterpret_cast<uint8_t *>(&value)[2] = readByte();
        reinterpret_cast<uint8_t *>(&value)[1] = readByte();
        reinterpret_cast<uint8_t *>(&value)[0] = readByte();
        return value;
    }

    void writeFullInt(int value)
    {
        writeByte(reinterpret_cast<uint8_t *>(&value)[3]);
        writeByte(reinterpret_cast<uint8_t *>(&value)[2]);
        writeByte(reinterpret_cast<uint8_t *>(&value)[1]);
        writeByte(reinterpret_cast<uint8_t *>(&value)[0]);
    }

    tl::optional<std::string> readString()
    {
        return ghr::readString(__data, __position);
    }

    void writeString(tl::optional<std::string> value)
    {
        ghr::writeString(__data, __position, value);
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

} // namespace ghr

#endif // __MESSAGE_H__