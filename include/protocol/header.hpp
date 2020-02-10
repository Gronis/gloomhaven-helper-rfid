#ifndef __PROTOCOL_HEADER_H__
#define __PROTOCOL_HEADER_H__

#include <string>
#include <cstddef>

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


} // namespace protocol
} // namespace ghh

#endif // __PROTOCOL_HEADER_H__