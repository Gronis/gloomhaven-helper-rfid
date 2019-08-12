#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <vector>

#include "print.hpp"

namespace ghr
{
enum class Condition
{
    value1,
    value2,
    Summon,
    Stuned,
    Immobilized,
    Disarmed,
    Wounded,
    Muddle,
    Poisoned,
    Strengthen,
    Invisible,
    value12,
    value13,
    value14,
    value15,
    value16,
    value17,
    value18,
    value19
};
std::vector<Condition> getConditionValues()
{
    return {
        Condition::value1,
        Condition::value2,
        Condition::Summon,
        Condition::Stuned,
        Condition::Immobilized,
        Condition::Disarmed,
        Condition::Wounded,
        Condition::Muddle,
        Condition::Poisoned,
        Condition::Strengthen,
        Condition::Invisible,
        Condition::value12,
        Condition::value13,
        Condition::value14,
        Condition::value15,
        Condition::value16,
        Condition::value17,
        Condition::value18,
        Condition::value19};
}

void print(Condition &arg)
{
    switch (arg)
    {
    case Condition::value1:
        ghr::print("value1");
        break;
    case Condition::value2:
        ghr::print("value2");
        break;
    case Condition::Summon:
        ghr::print("Summon");
        break;
    case Condition::Stuned:
        ghr::print("Stuned");
        break;
    case Condition::Immobilized:
        ghr::print("Immobilized");
        break;
    case Condition::Disarmed:
        ghr::print("Disarmed");
        break;
    case Condition::Wounded:
        ghr::print("Wounded");
        break;
    case Condition::Muddle:
        ghr::print("Muddle");
        break;
    case Condition::Poisoned:
        ghr::print("Poisoned");
        break;
    case Condition::Strengthen:
        ghr::print("Strengthen");
        break;
    case Condition::Invisible:
        ghr::print("Invisible");
        break;
    case Condition::value12:
        ghr::print("value12");
        break;
    case Condition::value13:
        ghr::print("value13");
        break;
    case Condition::value14:
        ghr::print("value14");
        break;
    case Condition::value15:
        ghr::print("value15");
        break;
    case Condition::value16:
        ghr::print("value16");
        break;
    case Condition::value17:
        ghr::print("value17");
        break;
    case Condition::value18:
        ghr::print("value18");
        break;
    case Condition::value19:
        ghr::print("value19");
        break;
    }
}

} // namespace ghr

#endif // __CONDITION_H__