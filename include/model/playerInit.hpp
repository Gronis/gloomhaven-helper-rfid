#ifndef __PLAYER_INIT_H__
#define __PLAYER_INIT_H__

#include <vector>

#include "print.hpp"

namespace ghh
{
struct PlayerInitNS
{
    enum Value{
        // These are guessed
        value1,
        value2,
        value3,
        value4,
        value5,
    };
};

using PlayerInit = PlayerInitNS::Value;

inline std::vector<PlayerInit> getPlayerInitValues()
{
    return {
        PlayerInit::value1,
        PlayerInit::value2,
        PlayerInit::value3,
        PlayerInit::value4,
        PlayerInit::value5};
}

inline void print(const PlayerInit &arg)
{
    switch (arg)
    {
    case PlayerInit::value1:
        ghh::print("value1");
        break;
    case PlayerInit::value2:
        ghh::print("value2");
        break;
    case PlayerInit::value3:
        ghh::print("value3");
        break;
    case PlayerInit::value4:
        ghh::print("value4");
        break;
    case PlayerInit::value5:
        ghh::print("value5");
        break;
    }
}

} // namespace ghh

#endif // __PLAYER_INIT_H__