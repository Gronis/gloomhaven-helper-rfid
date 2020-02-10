#ifndef __SUMMON_COLOR_H__
#define __SUMMON_COLOR_H__

#include "print.hpp"

namespace ghh
{
struct SummonColorNS
{
    enum Value{
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
};

using SummonColor = SummonColorNS::Value;


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
        ghh::print("None");
        break;
    case SummonColor::Turquise:
        ghh::print("Turquise");
        break;
    case SummonColor::Green:
        ghh::print("Green");
        break;
    case SummonColor::Yellow:
        ghh::print("Yellow");
        break;
    case SummonColor::Orange:
        ghh::print("Orange");
        break;
    case SummonColor::White:
        ghh::print("White");
        break;
    case SummonColor::Purple:
        ghh::print("Purple");
        break;
    case SummonColor::Pink:
        ghh::print("Pink");
        break;
    case SummonColor::Red:
        ghh::print("Red");
        break;
    }
}

} // namespace ghh

#endif // __SUMMON_COLOR_H__