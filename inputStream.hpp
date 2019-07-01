#include <functional>
#include <string>
#include <queue>
#include <memory>

#include "optional.hpp"
#include "utils.hpp"

// Provide a read function which reads from a input stream, and returns one character if data is available
// or -1 if no data is available.

class InputStream
{
private:
    std::queue<std::function<void(void)>> _callbackUpdates;
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
            std::cout << "read: " << byte << "\n";
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
                std::cout << "peaked varint: " << result << "\n";
                return tl::optional<int32_t>(result);
            }
        }
        // This should never happen, (invalid 32-bit integer)
        return tl::nullopt;
    }

    std::size_t peakBytes(std::size_t count, uint8_t *res)
    {
        auto bufferSize = this->bufferSize();
        std::cout << "peaked bytes "
                  << "buffer size: " << bufferSize << "\n";

        std::size_t i = 0, ii = this->_bufferIndexStart;
        while (i < count && bufferSize > i)
        {
            res[i] = this->_buffer[ii];
            std::cout << "peaked bytes [" << i << "]: " << static_cast<int32_t>(this->_buffer[ii]) << "\n";
            i = i + 1;
            ii = (ii + 1) % this->_bufferCapacity;
        }
        return i;
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
            std::cout << "readVarint: nrBytes " << nrBytes << "\n";
            this->moveBuffer(nrBytes);
        }
        return data;
    }
    // Advances the start index of buffer
    bool readBytes(std::size_t count, uint8_t *res)
    {
        bool ok = this->peakBytes(count, res);
        if (ok)
        {
            this->moveBuffer(count);
        }
        return ok;
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

    void asyncReadUTFString(std::function<void(std::string)> callback)
    {
        // Need a shared pointer to be able to manipulate in callback and
        // Remember next time it's called in update
        auto stringLength = std::make_shared<tl::optional<int16_t>>(tl::nullopt);
        this->_callbackUpdates.push([=]() {
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
                bool ok = this->readBytes(size, reinterpret_cast<uint8_t *>(data));
                if (ok)
                {
                    std::string result;
                    result.assign(data, size);
                    callback(result);
                    this->_callbackUpdates.pop();
                }
                free(data);
            }
        });
    }

    void asyncReadVarint(std::function<void(int32_t)> callback)
    {
        this->_callbackUpdates.push([=]() {
            this->readAvailableData();
            auto ok = this->readVarint();
            if (ok)
            {
                callback(ok.value());
                this->_callbackUpdates.pop();
            }
        });
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