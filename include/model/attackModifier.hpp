#ifndef __ATTACK_MODIFIER_H__
#define __ATTACK_MODIFIER_H__

#include <vector>

#include "print.hpp"

namespace ghh
{
struct AttackModifierNS
{
    enum Value{
        Zero,
        Plus1,
        Plus2,
        Minus1,
        Minus2,
        Miss,
        Crit,
        Bless,
        Curse,
    };
};

using AttackModifier = AttackModifierNS::Value;

inline std::vector<AttackModifier> getAttackModifierValues()
{
    return {
        AttackModifier::Zero,
        AttackModifier::Plus1,
        AttackModifier::Plus2,
        AttackModifier::Minus1,
        AttackModifier::Minus2,
        AttackModifier::Miss,
        AttackModifier::Crit,
        AttackModifier::Bless,
        AttackModifier::Curse,
    };
}

inline void print(const AttackModifier &arg)
{
    switch (arg)
    {
    case AttackModifier::Zero:
        ghh::print("0");
        break;
    case AttackModifier::Plus1:
        ghh::print("+1");
        break;
    case AttackModifier::Plus2:
        ghh::print("+2");
        break;
    case AttackModifier::Minus1:
        ghh::print("-1");
        break;
    case AttackModifier::Minus2:
        ghh::print("-2");
        break;
    case AttackModifier::Miss:
        ghh::print("x0");
        break;
    case AttackModifier::Crit:
        ghh::print("x2");
        break;
    case AttackModifier::Bless:
        ghh::print("x2*");
        break;
    case AttackModifier::Curse:
        ghh::print("x0*");
        break;
    }
}

} // namespace ghh

#endif // __ATTACK_MODIFIER_H__