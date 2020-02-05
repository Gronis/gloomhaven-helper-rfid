#ifndef __MONSTER_TYPE_H__
#define __MONSTER_TYPE_H__

#include "print.hpp"
#include <vector>


namespace ghr
{
struct MonsterTypeNS{
    enum Value{
        Normal,
        Elite,
        value3,
        Summon,
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
        value16
    };
};

using MonsterType = MonsterTypeNS::Value;

std::vector<MonsterType> getMonsterTypeValues()
{
    return {
        MonsterType::Normal,
        MonsterType::Elite,
        MonsterType::value3,
        MonsterType::Summon,
        MonsterType::value5,
        MonsterType::value6,
        MonsterType::value7,
        MonsterType::value8,
        MonsterType::value9,
        MonsterType::value10,
        MonsterType::value11,
        MonsterType::value12,
        MonsterType::value13,
        MonsterType::value14,
        MonsterType::value15,
        MonsterType::value16};
}

void print(const MonsterType &arg)
{
    switch (arg)
    {
    case MonsterType::Normal:
        ghr::print("Normal");
        break;
    case MonsterType::Elite:
        ghr::print("Elite");
        break;
    case MonsterType::value3:
        ghr::print("value3");
        break;
    case MonsterType::Summon:
        ghr::print("Summon");
        break;
    case MonsterType::value5:
        ghr::print("value5");
        break;
    case MonsterType::value6:
        ghr::print("value6");
        break;
    case MonsterType::value7:
        ghr::print("value7");
        break;
    case MonsterType::value8:
        ghr::print("value8");
        break;
    case MonsterType::value9:
        ghr::print("value9");
        break;
    case MonsterType::value10:
        ghr::print("value10");
        break;
    case MonsterType::value11:
        ghr::print("value11");
        break;
    case MonsterType::value12:
        ghr::print("value12");
        break;
    case MonsterType::value13:
        ghr::print("value13");
        break;
    case MonsterType::value14:
        ghr::print("value14");
        break;
    case MonsterType::value15:
        ghr::print("value15");
        break;
    case MonsterType::value16:
        ghr::print("value16");
        break;
    }
}

} // namespace ghr
#endif // __MONSTER_TYPE_H__