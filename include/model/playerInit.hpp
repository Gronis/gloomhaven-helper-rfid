#ifndef __PLAYER_INIT_H__
#define __PLAYER_INIT_H__

#include <vector>

#include "print.hpp"

namespace ghh {
struct PlayerInitNS {
  enum Value {
    DragOrder,
    DragNumber,
    DragNumberRequired,
    Numpad,
    value5,
  };
};

using PlayerInit = PlayerInitNS::Value;

inline std::vector<PlayerInit> getPlayerInitValues() {
  return {PlayerInit::DragOrder, PlayerInit::DragNumber,
          PlayerInit::DragNumberRequired, PlayerInit::Numpad,
          PlayerInit::value5};
}

inline void print(const PlayerInit &arg) {
  switch (arg) {
    case PlayerInit::DragOrder:
      ghh::print("DragOrder");
      break;
    case PlayerInit::DragNumber:
      ghh::print("DragNumber");
      break;
    case PlayerInit::DragNumberRequired:
      ghh::print("DragNumberRequired");
      break;
    case PlayerInit::Numpad:
      ghh::print("Numpad");
      break;
    case PlayerInit::value5:
      ghh::print("value5");
      break;
  }
}

}  // namespace ghh

#endif  // __PLAYER_INIT_H__