#ifndef __PROTOCOL_HEADER_H__
#define __PROTOCOL_HEADER_H__

#include <string>
#include <cstddef>

#include "protocol/buffer.hpp"

namespace ghh
{
namespace protocol
{

struct Header
{
    std::string event;
    std::string payload;
    std::size_t length;
};

bool readHeader(ghh::protocol::Header &state, ghh::protocol::Buffer &buffer);
void writeHeader(const ghh::protocol::Header &state, ghh::protocol::Buffer &buffer);

} // namespace protocol
} // namespace ghh

#endif // __PROTOCOL_HEADER_H__