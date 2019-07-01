#include <functional>
#include <string>
#include <queue>
#include <memory>

#include "optional.hpp"

// Provide a read function which reads from a stream, and returns one character if data is available
// or -1 if no data is available.

class Stream
{
private:
    // constexpr uint8_t CALLBACK_STRING = 1;
    // constexpr uint8_t CALLBACK_INT = 2;
    // constexpr uint8_t CALLBACK_BYTES = 3;
    // struct CallbackFunction{
    //     uint8_t callbackType;
    //     union{
    //         std::function<void(std::string)> callbackString;
    //         std::function<void(int32_t)> callbackVarint;
    //         struct{
    //             std::size_t nrBytes;
    //             std::function<void(uin8_t *)> callbackBytes;
    //         }
    //     }
    // };
    std::queue<std::function<void(void)>> _callbackUpdates;
    const std::function<int32_t(void)> _read;
    uint8_t *const _buffer;
    const std::size_t _bufferCapacity;
    std::size_t _bufferIndexStart = 0;
    std::size_t _bufferIndexEnd = 0;

    void reverse(std::size_t count, uint8_t *data)
    {
        std::size_t i = 0, ii = count - 1;
        uint8_t tmp;
        while (i < ii)
        {
            tmp = data[i];
            data[i] = data[ii];
            data[ii] = tmp;
            i++;
            ii--;
        }
    }

    std::size_t bufferSize()
    {
        return (this->_bufferIndexEnd - this->_bufferIndexStart + this->_bufferCapacity) % this->_bufferCapacity;
    }

    void moveBuffer(int32_t steps)
    {
        this->_bufferIndexStart = (this->_bufferIndexStart + steps + this->_bufferCapacity) % this->_bufferCapacity;
        std::cout << "moved buffer size: " << this->bufferSize() << "\n";
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

    // All peak functions does not advance the buffer
    tl::optional<uint8_t> peakByte()
    {
        auto bufferSize = this->bufferSize();
        uint8_t data;
        if (bufferSize >= 1)
        {
            peakBytes(1, &data);
            return tl::optional<uint8_t>(data);
        }
        return tl::nullopt;
    }

    tl::optional<int16_t> peakShort()
    {
        auto bufferSize = this->bufferSize();
        uint16_t data;
        if (bufferSize >= 2)
        {
            uint8_t *dataAsArray = reinterpret_cast<uint8_t *>(&data);
            peakBytes(2, dataAsArray);
            reverse(2, dataAsArray);
            std::cout << "peaked short: " << data << "\n";
            return tl::optional<int16_t>(data);
        }
        return tl::nullopt;
    }

    tl::optional<int32_t> peakInt()
    {
        auto bufferSize = this->bufferSize();
        uint32_t data;
        if (bufferSize >= 4)
        {
            uint8_t *dataAsArray = reinterpret_cast<uint8_t *>(&data);
            peakBytes(4, dataAsArray);
            reverse(4, dataAsArray);
            return tl::optional<int32_t>(data);
        }
        return tl::nullopt;
    }

    tl::optional<int32_t> peakVarint(bool dontKnow)
    {
        return tl::nullopt;
    }

    void peakBytes(std::size_t count, uint8_t *res)
    {
        auto bufferSize = this->bufferSize();
        std::cout << "peaked bytes "
                  << "buffer size: " << bufferSize << "\n";
        if (bufferSize >= count)
        {
            std::size_t i = 0, ii = this->_bufferIndexStart;
            while (i < count)
            {
                res[i] = this->_buffer[ii];
                std::cout << "peaked bytes [" << i << "]: " << static_cast<int32_t>(this->_buffer[ii]) << "\n";
                i = i + 1;
                ii = (ii + 1) % this->_bufferCapacity;
            }
        }
        else
        {
            // TODO: handle wait (not enouth bytes on buffer)
        }
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

    // Advances the start index of buffer
    void readBytes(std::size_t count, uint8_t *res)
    {
        this->peakBytes(count, res);
        this->moveBuffer(count);
    }

public:
    Stream(
        const std::function<int32_t(void)> read,
        uint8_t *const buffer,
        const std::size_t bufferCapacity)
        : _read(read),
          _buffer(buffer),
          _bufferCapacity(bufferCapacity)
    {
    }

    void asyncReadUTFString(std::function<void(std::string)> callback)
    {
        // Need a shared pointer to be able to manipulate in callback and
        // Remember next time it's called in update
        auto stringLength = std::make_shared<tl::optional<int16_t>>(tl::nullopt);
        this->_callbackUpdates.push([=]() -> void {
            while (true)
            {
                auto byte = this->_read();
                std::cout << "read: " << byte << "\n";
                auto done = byte == -1;
                if (done)
                    break;
                // TODO: verify that byte never is > 255 (it should not be)
                this->pushByte(static_cast<uint8_t>(byte));
            }
            if (!(*stringLength))
            {
                *stringLength = readShort();
            }
            std::cout << "buffer size: " << this->bufferSize() << "\n";
            auto done = *stringLength && this->bufferSize() >= stringLength->value();
            if (done)
            {
                auto size = static_cast<std::size_t>(stringLength->value());
                char *data = new char[size];
                std::cout << "done: size " << size << "\n";
                this->readBytes(size, reinterpret_cast<uint8_t *>(data));
                std::string result;
                result.assign(data, size);
                callback(result);
                free(data);
                this->_callbackUpdates.pop();
            }
        });
        std::cout << "Added to callback\n";
    }

    void asyncReadVarint(std::function<void(int32_t)> callback)
    {
    }

    void asyncReadBytes(std::size_t nrBytes, std::function<void(uint8_t *)> callback)
    {
    }

    void update()
    {
        if (!this->_callbackUpdates.empty())
        {
            std::cout << "Running Update\n";
            auto updateFunction = this->_callbackUpdates.front();
            updateFunction();
        }
    }
};