#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdint.h>
#include <string>

#include "encoding.hpp"

namespace ghr
{

class Message
{
private:
    uint8_t *__data;
    std::size_t __dataLength;
    std::size_t __position;

public:
    Message(
        uint8_t *data,
        std::size_t dataLength)
        : __data(data),
          __dataLength(dataLength),
          __position(0)
    {
    }

    int readInt(bool optimizePositive)
    {
        return ghr::readInt(this->__data, this->__position, optimizePositive);
    }

    void writeInt(int value, bool optimizePositive)
    {
        ghr::writeInt(this->__data, this->__position, value, optimizePositive);
    }

    bool readBoolean()
    {
        return ghr::readBoolean(this->__data, this->__position);
    }

    void writeBoolean(bool value)
    {
        ghr::writeBoolean(this->__data, this->__position, value);
    }
};

} // namespace ghr

#endif // __MESSAGE_H__