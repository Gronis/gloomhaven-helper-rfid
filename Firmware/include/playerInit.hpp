#ifndef __PLAYER_INIT_H__
#define __PLAYER_INIT_H__

#include <vector>
#include <iostream>

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

} // namespace ghr

std::ostream &operator<<(std::ostream &os, ghr::PlayerInit const &p)
{
    switch (p)
    {
    case ghr::PlayerInit::PLANNING_ROUND:
        return os << "planning round";
    case ghr::PlayerInit::PLAYING_ROUND:
        return os << "playing round";
    case ghr::PlayerInit::value3:
        return os << "value3";
    case ghr::PlayerInit::value4:
        return os << "value4";
    case ghr::PlayerInit::value5:
        return os << "value5";
    }
    return os;
}

#endif // __PLAYER_INIT_H__