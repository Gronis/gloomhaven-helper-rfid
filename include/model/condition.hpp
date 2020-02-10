#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <vector>

#include "print.hpp"

namespace ghh
{
struct ConditionNS
{
    enum Value {
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
};

using Condition = ConditionNS::Value;

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

void print(const Condition &arg)
{
    switch (arg)
    {
    case Condition::value1:
        ghh::print("value1");
        break;
    case Condition::value2:
        ghh::print("value2");
        break;
    case Condition::Summon:
        ghh::print("Summon");
        break;
    case Condition::Stuned:
        ghh::print("Stuned");
        break;
    case Condition::Immobilized:
        ghh::print("Immobilized");
        break;
    case Condition::Disarmed:
        ghh::print("Disarmed");
        break;
    case Condition::Wounded:
        ghh::print("Wounded");
        break;
    case Condition::Muddle:
        ghh::print("Muddle");
        break;
    case Condition::Poisoned:
        ghh::print("Poisoned");
        break;
    case Condition::Strengthen:
        ghh::print("Strengthen");
        break;
    case Condition::Invisible:
        ghh::print("Invisible");
        break;
    case Condition::value12:
        ghh::print("value12");
        break;
    case Condition::value13:
        ghh::print("value13");
        break;
    case Condition::value14:
        ghh::print("value14");
        break;
    case Condition::value15:
        ghh::print("value15");
        break;
    case Condition::value16:
        ghh::print("value16");
        break;
    case Condition::value17:
        ghh::print("value17");
        break;
    case Condition::value18:
        ghh::print("value18");
        break;
    case Condition::value19:
        ghh::print("value19");
        break;
    }
}

} // namespace ghh

#endif // __CONDITION_H__