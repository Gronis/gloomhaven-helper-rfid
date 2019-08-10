#ifndef __PLAYER_INIT_H__
#define __PLAYER_INIT_H__

#include <vector>

#include "print.hpp"

namespace ghr
{
enum class PlayerInit
{
    // These are guessed
    PLANNING_ROUND,
    PLAYING_ROUND,
    value3,
    value4,
    value5

};
std::vector<PlayerInit> getPlayerInitValues()
{
    return {
        PlayerInit::PLANNING_ROUND,
        PlayerInit::PLAYING_ROUND,
        PlayerInit::value3,
        PlayerInit::value4,
        PlayerInit::value5};
}

void print(PlayerInit &arg)
{
    switch (arg)
    {
    case PlayerInit::PLANNING_ROUND:
        ghr::print("planning round");
        break;
    case PlayerInit::PLAYING_ROUND:
        ghr::print("playing round");
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