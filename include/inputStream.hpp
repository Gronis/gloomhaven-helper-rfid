#ifndef __INPUT_STREAM_H__
#define __INPUT_STREAM_H__

#include <functional>
#include <string>
#include <queue>
#include <memory>

#include "print.hpp"
#include "optional.hpp"
#include "utils.hpp"

// Provide a read function which reads from a input stream, and returns one character if data is available
// or -1 if no data is available.

namespace ghh
{

class InputStream
{
private:
    std::queue<std::function<bool(void)>> _taskQueue;
    const std::function<int32_t(void)> _read;
    uint8_t *const _buffer;
    const std::size_t _bufferCapacity;
    std::size_t _bufferIndexStart = 0;
    std::size_t _bufferIndexEnd = 0;

    void readAvailableData()
    {
        while (true)
        {
            auto byte = this->_read();
            auto done = byte == -1;
            if (done)
                break;
            // TODO: verify that byte never is > 255 (it should not be)
            this->pushByte(static_cast<uint8_t>(byte));
        }
    }

    std::size_t bufferSize()
    {
        return (this->_bufferIndexEnd - this->_bufferIndexStart + this->_bufferCapacity) % this->_bufferCapacity;
    }

    void moveBuffer(int32_t steps)
    {
        this->_bufferIndexStart = (this->_bufferIndexStart + steps + this->_bufferCapacity) % this->_bufferCapacity;
        // TODO: Check and report of we move passed the buffer end. (Buffer underflow?)
    }

    // Advances the end index of buffer
    void pushByte(uint8_t byte)
    {
        auto bufferSize = this->bufferSize();
        if (bufferSize + 1 < this->_bufferCapacity)
        {
            this->_buffer[this->_bufferIndexEnd] = byte;
            this->_bufferIndexEnd = (this->_bufferIndexEnd + 1) % this->_bufferCapacity;
        }
        else
        {
            // TODO: Error buffer overflow
        }
    }

    tl::optional<uint8_t> peakByte()
    {
        auto bufferSize = this->bufferSize();
        uint8_t data;
        std::size_t count = 1;
        if (bufferSize >= count)
        {
            std::size_t actualCount = peakBytes(count, &data);
            return actualCount == count ? tl::optional<uint8_t>(data) : tl::nullopt;
        }
        return tl::nullopt;
    }

    tl::optional<int16_t> peakShort()
    {
        auto bufferSize = this->bufferSize();
        uint16_t data;
        std::size_t count = 2;
        if (bufferSize >= count)
        {
            uint8_t *dataAsArray = reinterpret_cast<uint8_t *>(&data);
            std::size_t actualCount = peakBytes(count, dataAsArray);
            reverse(count, dataAsArray);
            return actualCount == count ? tl::optional<int16_t>(data) : tl::nullopt;
        }
        return tl::nullopt;
    }

    tl::optional<int32_t> peakInt()
    {
        auto bufferSize = this->bufferSize();
        uint32_t data;
        std::size_t count = 4;
        if (bufferSize >= count)
        {
            uint8_t *dataAsArray = reinterpret_cast<uint8_t *>(&data);
            std::size_t actualCount = peakBytes(count, dataAsArray);
            reverse(count, dataAsArray);
            return actualCount == count ? tl::optional<int32_t>(data) : tl::nullopt;
        }
        return tl::nullopt;
    }

    tl::optional<int32_t> peakVarint()
    {
        int32_t b = 0;
        int32_t result = 0;
        uint8_t varIntBuffer[5];
        auto peekedBytes = peakBytes(5, varIntBuffer);
        for (auto i = 0; i <= 4; i++)
        {
            if (peekedBytes <= i)
                return tl::nullopt;

            uint8_t b = varIntBuffer[i];
            result |= (b & 0x7F) << (i * 7);
            if ((b & 0x80) == 0)
            {
                return tl::optional<int32_t>(result);
            }
        }
        // This should never happen, (invalid 32-bit integer)
        return tl::nullopt;
    }

    std::size_t peakBytes(std::size_t count, uint8_t *res)
    {
        auto bufferSize = this->bufferSize();

        std::size_t i = 0, ii = this->_bufferIndexStart;
        while (i < count && bufferSize > i)
        {
            res[i] = this->_buffer[ii];
            i = i + 1;
            ii = (ii + 1) % this->_bufferCapacity;
        }
        return i;
    }

    tl::optional<int8_t> readByte()
    {
        auto value = this->peakByte();
        if (value)
        {
            this->moveBuffer(1);
        }
        return value;
    }

    tl::optional<int16_t> readShort()
    {
        auto value = this->peakShort();
        if (value)
        {
            this->moveBuffer(2);
        }
        return value;
    }

    tl::optional<int32_t> readInt()
    {
        auto value = this->peakInt();
        if (value)
        {
            this->moveBuffer(4);
        }
        return value;
    }

    tl::optional<int32_t> readVarint()
    {
        auto data = this->peakVarint();
        if (data)
        {
            // Calculate how many bytes where needed when peeking
            auto nrBytes = 1;
            while (data.value() >> (nrBytes * 7) != 0)
            {
                nrBytes++;
            }
            this->moveBuffer(nrBytes);
        }
        return data;
    }
    // Advances the start index of buffer
    bool readBytes(std::size_t count, uint8_t *res)
    {
        std::size_t actualCount = this->peakBytes(count, res);
        if (actualCount == count)
        {
            this->moveBuffer(count);
            return true;
        }
        return false;
        ;
    }

public:
    InputStream(
        const std::function<int32_t(void)> read,
        uint8_t *const buffer,
        const std::size_t bufferCapacity)
        : _read(read),
          _buffer(buffer),
          _bufferCapacity(bufferCapacity)
    {
    }

    void queueReadUTFString(std::function<void(std::string)> callback)
    {
        // Need a shared pointer to be able to manipulate in callback and
        // Remember next time it's called in update
        auto stringLength = std::make_shared<tl::optional<int16_t>>(tl::nullopt);
        this->_taskQueue.push([=]() {
            this->readAvailableData();
            if (!*stringLength)
                *stringLength = readShort();
            auto done = *stringLength && this->bufferSize() >= stringLength->value();
            if (done)
            {
                auto size = static_cast<std::size_t>(stringLength->value());
                // TODO: Prone to errors if throwing exceptions in callback function
                // Fix somehow.
                char *data = new char[size];
                int32_t ok = this->readBytes(size, reinterpret_cast<uint8_t *>(data));
                if (ok)
                {
                    std::string result;
                    result.assign(data, size);
                    callback(result);
                    this->_taskQueue.pop();
                    return true;
                }
                free(data);
            }
            return false;
        });
    }

    void queueReadVarint(std::function<void(int32_t)> callback)
    {
        this->_taskQueue.push([=]() {
            this->readAvailableData();
            auto ok = this->readVarint();
            if (ok)
            {
                callback(ok.value());
                this->_taskQueue.pop();
                return true;
            }
            return false;
        });
    }

    void queueReadBytes(std::size_t count, std::function<void(uint8_t *)> callback)
    {
        this->_taskQueue.push([=]() {
            this->readAvailableData();
            bool done = this->bufferSize() >= count;
            if (done)
            {
                // TODO: Prone to errors if throwing exceptions in callback function
                // Fix somehow.
                uint8_t *data = new uint8_t[count];
                bool ok = this->readBytes(count, data);
                if (ok)
                {
                    callback(data);
                    this->_taskQueue.pop();
                }
                free(data);
                return true;
            }
            return false;
        });
    }

    void update(std::function<void(void)> ifEmptyCallThis)
    {
        auto stalled = false;
        while (!stalled && !this->_taskQueue.empty())
        {
            auto task = this->_taskQueue.front();
            auto done = task();
            stalled = !done;
        }
        if (this->_taskQueue.empty())
        {
            ifEmptyCallThis();
        }
    }
};

} // namespace ghh

#endif // __INPUT_STREAM_H__
