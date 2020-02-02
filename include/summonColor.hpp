#ifndef __SUMMON_COLOR_H__
#define __SUMMON_COLOR_H__

#include "print.hpp"

namespace ghr
{
enum class SummonColor
{
    None,
    Turquise,
    Green,
    Yellow,
    Orange,
    White,
    Purple,
    Pink,
    Red,
};
std::vector<SummonColor> getSummonColorValues()
{
    return {
        SummonColor::None,
        SummonColor::Turquise,
        SummonColor::Green,
        SummonColor::Yellow,
        SummonColor::Orange,
        SummonColor::White,
        SummonColor::Purple,
        SummonColor::Pink,
        SummonColor::Red,
    };
}

void print(const SummonColor &arg)
{
    switch (arg)
    {
    case SummonColor::None:
        ghr::print("None");
        break;
    case SummonColor::Turquise:
        ghr::print("Turquise");
        break;
    case SummonColor::Green:
        ghr::print("Green");
        break;
    case SummonColor::Yellow:
        ghr::print("Yellow");
        break;
    case SummonColor::Orange:
        ghr::print("Orange");
        break;
    case SummonColor::White:
        ghr::print("White");
        break;
    case SummonColor::Purple:
        ghr::print("Purple");
        break;
    case SummonColor::Pink:
        ghr::print("Pink");
        break;
    case SummonColor::Red:
        ghr::print("Red");
        break;
    }
}

} // namespace ghr

#endif // __SUMMON_COLOR_H__