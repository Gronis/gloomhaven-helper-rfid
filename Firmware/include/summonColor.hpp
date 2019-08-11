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
    value4,
    value5,
    value6,
    value7,
    value8,
    value9,
    value10,
    value11,
    value12,
    value13,
    value14,
    value15,
    value16,
    value17,
    value18
};
std::vector<SummonColor> getSummonColorValues()
{
    return {
        SummonColor::None,
        SummonColor::Turquise,
        SummonColor::Green,
        SummonColor::value4,
        SummonColor::value5,
        SummonColor::value6,
        SummonColor::value7,
        SummonColor::value8,
        SummonColor::value9,
        SummonColor::value10,
        SummonColor::value11,
        SummonColor::value12,
        SummonColor::value13,
        SummonColor::value14,
        SummonColor::value15,
        SummonColor::value16,
        SummonColor::value17,
        SummonColor::value18};
}

void print(SummonColor &arg)
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
    case SummonColor::value4:
        ghr::print("value4");
        break;
    case SummonColor::value5:
        ghr::print("value5");
        break;
    case SummonColor::value6:
        ghr::print("value6");
        break;
    case SummonColor::value7:
        ghr::print("value7");
        break;
    case SummonColor::value8:
        ghr::print("value8");
        break;
    case SummonColor::value9:
        ghr::print("value9");
        break;
    case SummonColor::value10:
        ghr::print("value10");
        break;
    case SummonColor::value11:
        ghr::print("value11");
        break;
    case SummonColor::value12:
        ghr::print("value12");
        break;
    case SummonColor::value13:
        ghr::print("value13");
        break;
    case SummonColor::value14:
        ghr::print("value14");
        break;
    case SummonColor::value15:
        ghr::print("value15");
        break;
    case SummonColor::value16:
        ghr::print("value16");
        break;
    case SummonColor::value17:
        ghr::print("value17");
        break;
    case SummonColor::value18:
        ghr::print("value18");
        break;
    }
}

} // namespace ghr

#endif // __SUMMON_COLOR_H__