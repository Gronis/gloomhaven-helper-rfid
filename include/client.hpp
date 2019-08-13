#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>
#include "inputStream.hpp"
#include "outputStream.hpp"

namespace ghr
{

class Client
{
private:
    InputStream __input;
    OutputStream __output;

    void __read_header()
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
                    this->on_data(*event, *payload, data, dataLength);
                });
            }
            else
            {
                this->on_data(*event, *payload, nullptr, 0);
            }
        });
    }

    void __write_header(std::string event, std::string payload, std::size_t dataLength)
    {
        std::string msg = (payload.length() > 0) ? (event + " " + payload) : event;
        __output.writeUTFString(msg);
        __output.writeVarint(dataLength);
    }

public:
    std::function<void(std::string, std::string, uint8_t *, std::size_t)> on_data;
    Client(
        InputStream input,
        OutputStream output) : __input(input),
                               __output(output) {}

    void send_data(std::string event, std::string payload, uint8_t *data, std::size_t dataLength)
    {
        //We have one byte = 0 extra in the end of the message
        __write_header(event, payload, dataLength + 1);
        __output.writeBytes(data, dataLength);
        uint8_t zero[3] = {0, 0, 0};
        __output.writeBytes(zero, 1);
    }

    void update()
    {
        this->__input.update(std::bind(&Client::__read_header, this));
    }
};
} // namespace ghr
#endif // __CLIENT_H__