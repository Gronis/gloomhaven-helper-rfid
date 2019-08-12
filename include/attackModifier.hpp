#ifndef __ATTACK_MODIFIER_H__
#define __ATTACK_MODIFIER_H__

#include <vector>

#include "print.hpp"

namespace ghr
{
enum class AttackModifier
{
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
std::vector<AttackModifier> getAttackModifierValues()
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

void print(AttackModifier &arg)
{
    switch (arg)
    {
    case AttackModifier::Zero:
        ghr::print("0");
        break;
    case AttackModifier::Plus1:
        ghr::print("+1");
        break;
    case AttackModifier::Plus2:
        ghr::print("+2");
        break;
    case AttackModifier::Minus1:
        ghr::print("-1");
        break;
    case AttackModifier::Minus2:
        ghr::print("-2");
        break;
    case AttackModifier::Miss:
        ghr::print("x0");
        break;
    case AttackModifier::Crit:
        ghr::print("x2");
        break;
    case AttackModifier::Bless:
        ghr::print("x2*");
        break;
    case AttackModifier::Curse:
        ghr::print("x0*");
        break;
    }
}

} // namespace ghr

#endif // __ATTACK_MODIFIER_H__