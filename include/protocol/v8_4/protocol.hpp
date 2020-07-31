#ifndef __PROTOCOL_V8_4_PROTOCOL_H__
#define __PROTOCOL_V8_4_PROTOCOL_H__

#include "model/gameState.hpp"
#include "protocol/buffer.hpp"

namespace ghh {
namespace protocol {
namespace v8_4 {

void readGameState(ghh::GameState &state, ghh::protocol::Buffer &buffer);
void writeGameState(const ghh::GameState &state, ghh::protocol::Buffer &buffer);

}  // namespace v8_4
}  // namespace protocol
}  // namespace ghh

#endif  // __PROTOCOL_V8_4_PROTOCOL_H__