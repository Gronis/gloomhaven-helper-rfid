#ifndef __ATTACK_MODIFIER_H__
#define __ATTACK_MODIFIER_H__

#include <vector>

#include "print.hpp"

namespace ghr
{
enum class AttackModifier
{
    value1,
    value2,
    value3,
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
    value18,
    value19,
    value20,
    value21,
    value22,
};
std::vector<AttackModifier> getAttackModifierValues()
{
    return {
        AttackModifier::value1,
        AttackModifier::value2,
        AttackModifier::value3,
        AttackModifier::value4,
        AttackModifier::value5,
        AttackModifier::value6,
        AttackModifier::value7,
        AttackModifier::value8,
        AttackModifier::value9,
        AttackModifier::value10,
        AttackModifier::value11,
        AttackModifier::value12,
        AttackModifier::value13,
        AttackModifier::value14,
        AttackModifier::value15,
        AttackModifier::value16,
        AttackModifier::value17,
        AttackModifier::value18,
        AttackModifier::value19,
        AttackModifier::value20,
        AttackModifier::value21,
        AttackModifier::value22};
}

void print(AttackModifier &arg)
{
    ghr::print((unsigned int)arg);
}

} // namespace ghr

#endif // __ATTACK_MODIFIER_H__