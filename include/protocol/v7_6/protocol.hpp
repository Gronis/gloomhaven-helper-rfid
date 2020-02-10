#ifndef __PROTOCOL_V7_6_PROTOCOL_H__
#define __PROTOCOL_V7_6_PROTOCOL_H__

#include "protocol/buffer.hpp"
#include "model/gameState.hpp"

namespace ghh
{
namespace protocol
{
namespace v7_6
{

void readGameState(ghh::GameState &state, ghh::protocol::Buffer &msg);
void writeGameState(const ghh::GameState &state, ghh::protocol::Buffer &msg);

} // namespace v7_6
} // namespace protocol
} // namespace ghh

#endif // __PROTOCOL_V7_6_PROTOCOL_H__