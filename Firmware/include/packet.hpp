#ifndef __PACKET_H__
#define __PACKET_H__

#include <string>
#include "inputStream.hpp"

namespace ghr
{

class Packet
{
private:
    std::function<void(std::string, std::string, uint8_t *, std::size_t)> __on_data;
    InputStream __input;

    void __read_packet()
    {
        auto event = std::make_shared<std::string>();
        auto payload = std::make_shared<std::string>();
        this->__input.queueReadUTFString([=](std::string message) {
            std::size_t index = message.find(" ");
            if (index != -1)
            {
                *event = trim(message.substr(0, index));
                *payload = trim(message.substr(index + 1));
            }
            else
            {
                *event = trim(message);
                *payload = "";
            }
        });
        this->__input.queueReadVarint([=](int32_t dataLength) {
            if (dataLength > 0)
            {
                this->__input.queueReadBytes(dataLength, [=](uint8_t *data) {
                    this->__on_data(*event, *payload, data, dataLength);
                });
            }
            else
            {
                this->__on_data(*event, *payload, nullptr, 0);
            }
        });
    }

public:
    Packet(
        std::function<void(std::string, std::string, uint8_t *, std::size_t)> on_data,
        InputStream input) : __on_data(on_data), __input(input)
    {
    }
    void update()
    {
        this->__input.update(std::bind(&Packet::__read_packet, this));
    }
};
} // namespace ghr
#endif // __PACKET_H__