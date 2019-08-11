#ifndef __MONSTER_ACTOR_H__
#define __MONSTER_ACTOR_H__

#include "print.hpp"

namespace ghr
{
struct MonsterActor
{
    int data;
    int id;
    bool is_normal;
    bool is_elite;
    MonsterActor() : data(0), id(0), is_normal(false), is_elite(false) {}
};

void print(MonsterActor &arg)
{
    print("  {\n");
    print("    data:      ", arg.data, "\n");
    print("    id:        ", arg.id, "\n");
    print("    is_normal: ", arg.is_normal, "\n");
    print("    is_elite:  ", arg.is_elite, "\n");
    print("  }\n");
}

} // namespace ghr

#endif // __MONSTER_ACTOR_H__