#ifndef __OUTPUT_STREAM_H__
#define __OUTPUT_STREAM_H__

#include <functional>
#include <string>
#include <queue>
#include <memory>

#include "print.hpp"
#include "optional.hpp"
#include "utils.hpp"

#include "protocol/encoding.hpp"

// Provide a read function which reads from a input stream, and returns one character if data is available
// or -1 if no data is available.

namespace ghr
{

class OutputStream
{
private:
    const std::function<int32_t(uint8_t)> _write;
    uint8_t *const _buffer;
    const std::size_t _bufferCapacity;

    void __write_buffer(const std::size_t length)
    {
        for (std::size_t i = 0; i < length; i++)
        {
            _write(_buffer[i]);
        }
    }

    void __writeShort(int16_t value)
    {
        _buffer[0] = (uint8_t)(value >> 8);
        _buffer[1] = (uint8_t)(value);
        __write_buffer(2);
    }

public:
    OutputStream(
        const std::function<uint8_t(uint8_t)> write,
        uint8_t *const buffer,
        const std::size_t bufferCapacity)
        : _write(write),
          _buffer(buffer),
          _bufferCapacity(bufferCapacity)
    {
    }
    void writeUTFString(std::string value)
    {
        __writeShort(value.length());
        const uint8_t *data = reinterpret_cast<const uint8_t *>(value.c_str());
        writeBytes(data, value.length());
    }
    void writeVarint(int value)
    {
        std::size_t pos = 0;
        ghr::writeInt(_buffer, pos, value, true);
        __write_buffer(pos);
    }
    void writeBytes(const uint8_t *data, const std::size_t dataLength)
    {
        for (std::size_t i = 0; i < dataLength; i++)
        {
            _buffer[i] = data[i];
        }
        __write_buffer(dataLength);
    }
};

} // namespace ghr

#endif // __OUTPUT_STREAM_H__