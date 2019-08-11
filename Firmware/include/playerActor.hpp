#ifndef __PLAYER_ACTOR_H__
#define __PLAYER_ACTOR_H__

#include <string>
#include <vector>

#include "print.hpp"

#include "characterClass.hpp"

namespace ghr
{
struct PlayerActor
{
    std::string name;
    CharacterClass character_class;
    int xp;
    int hp;
    int hp_max;
    int level;
    int loot;
    int initiative;
    std::vector<Condition> conditions;
    std::vector<Condition> conditions_expired;
    std::vector<Condition> conditions_current_turn;
    bool exhausted;

    PlayerActor() : name(""),
                    character_class(CharacterClass::Escort),
                    xp(0),
                    hp(0),
                    hp_max(0),
                    level(0),
                    loot(0),
                    initiative(0),
                    conditions(),
                    conditions_expired(),
                    conditions_current_turn(),
                    exhausted(false)
    {
    }
};

void print(PlayerActor &arg)
{
    print("   {\n");
    print("       name: ", arg.name, "\n");
    print("       character_class: ", arg.character_class, "\n");
    print("       xp: ", arg.xp, "\n");
    print("       hp: ", arg.hp, "\n");
    print("       hp_max: ", arg.hp_max, "\n");
    print("       level: ", arg.level, "\n");
    print("       loot: ", arg.loot, "\n");
    print("       initiative: ", arg.initiative, "\n");
    print("       conditions: ", arg.conditions.size(), "\n");
    for (auto &&c : arg.conditions)
    {
        print("         ", c, "\n");
    }
    print("       conditions expired: ", arg.conditions_expired.size(), "\n");
    for (auto &&c : arg.conditions_expired)
    {
        print("         ", c, "\n");
    }
    print("       conditions current_turn: ", arg.conditions_current_turn.size(), "\n");
    for (auto &&c : arg.conditions_current_turn)
    {
        print("         ", c, "\n");
    }
    print("       exhausted: ", arg.exhausted, "\n");
    print("   }\n");
}

} // namespace ghr

#endif // __PLAYER_ACTOR_H__