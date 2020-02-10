#ifndef __PROTOCOL_V8_0_PROTOCOL_H__
#define __PROTOCOL_V8_0_PROTOCOL_H__

#include "protocol/buffer.hpp"
#include "model/gameState.hpp"

namespace ghh
{
namespace protocol
{
namespace v8_0
{

void readGameState(ghh::GameState &state, ghh::protocol::Buffer &buffer);
void writeGameState(const ghh::GameState &state, ghh::protocol::Buffer &buffer);

} // namespace v8_0
} // namespace protocol
} // namespace ghh

#endif // __PROTOCOL_V8_0_PROTOCOL_H__