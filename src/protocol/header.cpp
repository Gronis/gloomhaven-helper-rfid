#include "protocol/header.hpp"

#include "utils.hpp"

using namespace ghh;
using namespace ghh::protocol;

bool ghh::protocol::readHeader(Header &header, Buffer &buffer)
{
    auto start = buffer.getReadPosition();
    tl::optional<std::string> message = buffer.readUTFString();

    if(!message.has_value() || buffer.getSize() < 5){
        buffer.setReadPosition(start);
        return false;
    }
    int len = buffer.readInt(true);
    std::string msg = message.value();
    std::size_t index = msg.find(" ");
    if (index != -1)
    {
        header.event = trim(msg.substr(0, index));
        header.payload = trim(msg.substr(index + 1));
    }
    else
    {
        header.event = trim(msg);
        header.payload = "";
    }
    header.length = len;
    return true;
}

void ghh::protocol::writeHeader(const Header &header, Buffer &buffer)
{
    std::size_t count = 0;
    std::string msg =
        (header.payload.length() > 0) ? (header.event + " " + header.payload) : header.event;
    buffer.writeUTFString(msg);
    buffer.writeInt(header.length, true);
}