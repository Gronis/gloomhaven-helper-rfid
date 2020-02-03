#ifndef __PLAYER_INIT_H__
#define __PLAYER_INIT_H__

#include <vector>

#include "print.hpp"

namespace ghr
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

std::vector<PlayerInit> getPlayerInitValues()
{
    return {
        PlayerInit::value1,
        PlayerInit::value2,
        PlayerInit::value3,
        PlayerInit::value4,
        PlayerInit::value5};
}

void print(const PlayerInit &arg)
{
    switch (arg)
    {
    case PlayerInit::value1:
        ghr::print("value1");
        break;
    case PlayerInit::value2:
        ghr::print("value2");
        break;
    case PlayerInit::value3:
        ghr::print("value3");
        break;
    case PlayerInit::value4:
        ghr::print("value4");
        break;
    case PlayerInit::value5:
        ghr::print("value5");
        break;
    }
}

} // namespace ghr

#endif // __PLAYER_INIT_H__