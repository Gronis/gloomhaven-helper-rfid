#ifndef __PROTOCOL_HEADER_H__
#define __PROTOCOL_HEADER_H__

#include <vector>

#include "optional.hpp"

namespace ghr
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
} // namespace ghr

#endif // __PROTOCOL_HEADER_H__