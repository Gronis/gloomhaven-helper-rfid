#ifndef __MONSTER_INSTANCE_H__
#define __MONSTER_INSTANCE_H__

#include <vector>

#include "print.hpp"

#include "model/condition.hpp"
#include "model/monsterType.hpp"
#include "model/summonColor.hpp"

namespace ghr
{
struct MonsterInstance
{
    int number;
    MonsterType type;
    SummonColor summon_color;
    int summon_move;
    int summon_attack;
    int summon_range;
    bool is_new;
    int hp;
    int hp_max;
    std::vector<Condition> conditions;
    std::vector<Condition> conditions_expired;
    std::vector<Condition> conditions_current_turn;
    MonsterInstance() : number(0),
                        type(MonsterType::Normal),
                        summon_color(SummonColor::None),
                        summon_move(0),
                        summon_attack(0),
                        summon_range(0),
                        is_new(false),
                        hp(0),
                        hp_max(0),
                        conditions({}),
                        conditions_expired({}),
                        conditions_current_turn({})
    {
    }
};

void print(const MonsterInstance &arg)
{
    print("   {\n");
    print("       number: ", arg.number, "\n");
    print("       type: ", arg.type, "\n");
    if (arg.type == MonsterType::Summon)
    {
        print("       summon_color:  ", arg.summon_color, "\n");
        print("       summon_move:   ", arg.summon_move, "\n");
        print("       summon_attack: ", arg.summon_attack, "\n");
        print("       summon_range:  ", arg.summon_range, "\n");
    }
    print("       is_new: ", arg.is_new, "\n");
    print("       hp: ", arg.hp, "\n");
    print("       hp_max: ", arg.hp_max, "\n");
    print("       conditions: ", arg.conditions.size(), "\n");
    for (const auto c : arg.conditions)
    {
        print("         ", c, "\n");
    }
    print("       conditions expired: ", arg.conditions_expired.size(), "\n");
    for (const auto c : arg.conditions_expired)
    {
        print("         ", c, "\n");
    }
    print("       conditions current_turn: ", arg.conditions_current_turn.size(), "\n");
    for (const auto c : arg.conditions_current_turn)
    {
        print("         ", c, "\n");
    }
    print("   }\n");
}

} // namespace ghr

#endif // __MONSTER_INSTANCE_H__