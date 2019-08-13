#ifndef __MONSTER_ACTOR_H__
#define __MONSTER_ACTOR_H__

#include "print.hpp"

namespace ghr
{
struct MonsterActor
{
    int id;
    int level;
    bool is_normal;
    bool is_elite;
    tl::optional<int> ability;
    MonsterActor() : id(0), level(0), is_normal(false), is_elite(false), ability(tl::nullopt) {}
};

void print(MonsterActor &arg)
{
    print("  {\n");
    print("    id:        ", arg.id, "\n");
    print("    level:     ", arg.level, "\n");
    print("    is_normal: ", arg.is_normal, "\n");
    print("    is_elite:  ", arg.is_elite, "\n");
    print("    ability:  ", arg.ability ? arg.ability.value() : -1, "\n");
    print("  }\n");
}

} // namespace ghr

#endif // __MONSTER_ACTOR_H__